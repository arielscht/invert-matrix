#include <stdio.h>
#include <float.h>
#include <math.h>
#include <immintrin.h>

#include "../sisUtils/sisUtils.h"

#define SIMD_DBL_QTD 4

/*!
  \brief Encontra o pivô de uma coluna para a eliminação de gauss
  *
  \param A Ponteiro para a matriz de coeficientes
  \param curColumn O índice da coluna atual na eliminação
  \param size Tamanho da matriz de coeficientes
  *
  \returns O índice da linha do pivô encontrado
*/
uint findPivot(real_t **A,
               uint curColumn,
               uint size)
{
    uint pivot = curColumn;

    for (uint line = curColumn + 1; line < size; line++)
        if ((A[line][curColumn] - A[pivot][curColumn]) > DBL_EPSILON)
            pivot = line;

    return pivot;
}

/*!
  \brief Troca duas linhas de uma matriz do tipo ponteiro para ponteiro
  *
  \param matrix Ponteiro para a matriz
  \param line1 Índice da linha 1 a ser trocada
  \param line2 Índice da linha 2 a ser trocada
*/
void swapLines(real_t **matrix,
               uint line1,
               uint line2,
               uint size)
{
    for (uint i = 0; i < size; i++)
    {
        real_t aux = matrix[line1][i];
        matrix[line1][i] = matrix[line2][i];
        matrix[line2][i] = aux;
    }
}

/*!
  \brief Aplica a retrosubstituição em um sistema linear triangular superior
  *
  \param SL Ponteiro para o sistema linear
  \param solution Ponteiro para o array onde a solução será armazenada
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus retroSubstitution(real_t **matrix,
                                 real_t *indTerms,
                                 real_t *solution,
                                 uint size)
{
    FunctionStatus status = success;
    real_t mult;

    for (uint line = size; line >= 1; line--)
    {
        // variable used due to uint never being less than 0
        uint actualLine = line - 1;
        solution[actualLine] = indTerms[actualLine];
        for (uint column = size - 1; column > actualLine; column--)
        {
            mult = matrix[actualLine][column] * solution[column];
            solution[actualLine] -= mult;
        }
        solution[actualLine] = solution[actualLine] / matrix[actualLine][actualLine];
    }
    return status;
}

/*!
  \brief Aplica a retrosubstituição em um sistema linear triangular inferior
  *
  \param SL Ponteiro para o sistema linear
  \param solution Ponteiro para o array onde a solução será armazenada
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus reverseRetroSubstitution(real_t **matrix,
                                        real_t *indTerms,
                                        real_t *solution,
                                        uint size)
{
    FunctionStatus status = success;
    real_t mult;

    for (int line = 0; line < size; line++)
    {
        solution[line] = indTerms[line];
        for (int column = 0; column < line; column++)
        {
            mult = matrix[line][column] * solution[column];
            solution[line] -= mult;
        }
        solution[line] = solution[line] / matrix[line][line];
    }

    return status;
}

/*!
  \brief Calcula a norma L2 dos resíduos
  *
  \param residual Ponteiro para a matriz de residuos
  \param size Tamanho da matriz de residuos
  \param result Ponteiro para a varíavel de resultado
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcL2Norm(real_t **residual,
                          uint size,
                          real_t *result)
{
    real_t sum = 0.0;
    FunctionStatus status = success;
    int vectorSize = size / SIMD_DBL_QTD;
    int vectorLimit = size - (size % SIMD_DBL_QTD);
    __m256d aux;

    for (uint i = 0; i < size; i++)
    {
        __m256d *avxResidual = (__m256d *)(residual[i]);
        for (uint j = 0; j < vectorSize; j++)
        {
            aux = _mm256_mul_pd(avxResidual[j], avxResidual[j]);
            for (int ii = 0; ii < SIMD_DBL_QTD; ii++)
                sum += aux[ii];
        }

        for (uint j = vectorLimit; j < size; j++)
            sum += residual[i][j] * residual[i][j];
    }
    *result = sqrt(sum);
    return status;
}

/*!
  \brief Calcula o resíduo de um sistema linear
  *
  \param SL Ponteiro para o sistema linear
  \param solution Ponteiro para o array de soluções do sistema
  \param residual Ponteiro para o array onde os resíduos serão armazenados
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcResidual(real_t **matrix,
                            real_t *indTerms,
                            real_t *solution,
                            real_t *residual,
                            uint size)
{
    FunctionStatus status = success;
    real_t mult;

    for (size_t i = 0; i < size; i++)
    {
        residual[i] = indTerms[i];
        for (size_t j = 0; j < size; j++)
        {
            mult = solution[j] * matrix[i][j];
            residual[i] -= mult;
        }
    }

    return status;
}

/*!
  \brief Função que vai calcular o resíduo da iteração atual do refinamento
  *
  \param identity Ponteiro para a matriz identidade
  \param auxSL Ponteiro para o sistema auxiliar durante as contas
  \param solution Ponteiro para a matriz inversa
  \param curSol Ponteiro para o vetor de solução atual do refinamento
  \param residuals Ponteiro para a matriz de residuos
  \param size Tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcRefinementResidual(real_t **identity,
                                      real_t **matrix,
                                      real_t **solution,
                                      real_t **residuals,
                                      uint size)
{
    FunctionStatus status = success;
    int vectorSize = size / SIMD_DBL_QTD;
    int unrollLimit = size - (size % SIMD_DBL_QTD);
    __m256d aux;

    for (uint i = 0; i < size; i++)
    {
        for (uint j = 0; j < size; j++)
        {
            residuals[i][j] = identity[i][j];
            __m256d *avxSol = (__m256d *)(solution[i]);
            __m256d *avxMatrix = (__m256d *)(matrix[j]);

            for (uint k = 0; k < vectorSize; k++)
            {
                aux = _mm256_mul_pd(avxSol[k], avxMatrix[k]);
                for (int ii = 0; ii < SIMD_DBL_QTD; ii++)
                {
                    residuals[i][j] -= aux[ii];
                }
            }

            for (uint l = unrollLimit; l < size; l++)
                residuals[i][j] -= solution[i][l] * matrix[j][l];
        }
    }
    return status;
}

/*!
  \brief Função que vai calcular a nova aproximação do refinamento para a próxima iteração
  *
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param residuals Ponteiro para a matriz de residuos
  \param L Ponteiro para a matriz L
  \param auxSL Ponteiro para o sistema auxiliar durante as contas
  \param curSol Ponteiro para o vetor de solução atual do refinamento
  \param solution Ponteiro para o vetor de solução da interação anterior
  \param U Ponteiro para a matriz U
  \param size Tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcRefinementNewApproximation(uint *lineSwaps,
                                              real_t **residuals,
                                              real_t **L,
                                              real_t *curSol,
                                              real_t **solution,
                                              real_t **U,
                                              uint size)
{
    FunctionStatus status = success;

    for (int i = 0; i < size; i++)
    {
        applyLineSwapsOnArray(lineSwaps, residuals[i], size);

        reverseRetroSubstitution(L, residuals[i], curSol, size);

        retroSubstitution(U, curSol, curSol, size);
        // soma a solução do resíduo com a solução anterior para obter a nova apromixação
        for (int j = 0; j < size; j++)
            solution[i][j] += curSol[j];
    }

    return status;
}
