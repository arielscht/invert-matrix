#include <stdio.h>
#include <float.h>
#include <math.h>
#include "../sisUtils/sisUtils.h"

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
            if ((status = multiplyDouble(&mult, matrix[actualLine][column], solution[column])) != success)
                return status;
            solution[actualLine] -= mult;
        }
        if ((status = divideDouble(&solution[actualLine], solution[actualLine], matrix[actualLine][actualLine])) != success)
            return status;
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
            if ((status = multiplyDouble(&mult, matrix[line][column], solution[column])) != success)
                return status;
            solution[line] -= mult;
        }
        if ((status = divideDouble(&solution[line], solution[line], matrix[line][line])) != success)
            return status;
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
    real_t mult;

    for (uint i = 0; i < size; i++)
        for (uint j = 0; j < size; j++)
        {
            if ((status = multiplyDouble(&mult, residual[i][j], residual[i][j])) != success)
                return status;
            sum += mult;
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
            if ((status = multiplyDouble(&mult, solution[j], matrix[i][j])) != success)
                return status;
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

    for (int i = 0; i < size && status == success; i++)
    {
        status = calcResidual(matrix, identity[i], solution[i], residuals[i], size);
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

    for (int i = 0; i < size && status == success; i++)
    {
        applyLineSwapsOnArray(lineSwaps, residuals[i], size);

        if ((status = reverseRetroSubstitution(L, residuals[i], curSol, size)) != success)
            continue;

        if ((status = retroSubstitution(U, curSol, curSol, size)) != success)
            continue;
        // soma a solução do resíduo com a solução anterior para obter a nova apromixação
        for (int j = 0; j < size; j++)
            solution[i][j] += curSol[j];
    }

    return status;
}
