#include <stdio.h>
#include <float.h>
#include <math.h>
#include <immintrin.h>

#include "../sisUtils/sisUtils.h"

#define SIMD_DBL_QTD 4

double hsum_double_avx(__m256d v)
{
    __m128d vlow = _mm256_castpd256_pd128(v);
    __m128d vhigh = _mm256_extractf128_pd(v, 1); // high 128
    vlow = _mm_add_pd(vlow, vhigh);              // reduce down to 128

    __m128d high64 = _mm_unpackhi_pd(vlow, vlow);
    return _mm_cvtsd_f64(_mm_add_sd(vlow, high64)); // reduce to scalar
}

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
  \param size tamanho da matriz
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
  \param matrix matriz do sl
  \param indTerms termos independentes do sl
  \param solution solução do SL
  \param size tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus retroSubstitution(real_t **matrix,
                                 real_t *restrict indTerms,
                                 real_t *restrict solution,
                                 uint size)
{
    __m256d aux;

    for (uint line = size; line >= 1; line--)
    {
        // variable used due to uint never being less than 0
        int lineSize = size - line;
        int unrollLimit = size - (lineSize % SIMD_DBL_QTD);
        uint actualLine = line - 1;
        int vectorSize = lineSize / SIMD_DBL_QTD;
        solution[actualLine] = indTerms[actualLine];
        for (uint column = 0; column < vectorSize; column++)
        {
            int index = line + (column * SIMD_DBL_QTD);
            aux = _mm256_mul_pd(
                _mm256_loadu_pd(&solution[index]),
                _mm256_loadu_pd(&matrix[actualLine][index]));
            solution[actualLine] -= hsum_double_avx(aux);
        }

        for (uint k = unrollLimit; k < size; k++)
            solution[actualLine] -= matrix[actualLine][k] * solution[k];

        solution[actualLine] = solution[actualLine] / matrix[actualLine][actualLine];
    }
    return success;
}

/*!
  \brief Aplica a retrosubstituição em um sistema linear triangular inferior
  *
  \param matrix matriz do sl
  \param indTerms termos independentes do sl
  \param solution solução do SL
  \param size tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus reverseRetroSubstitution(real_t **matrix,
                                        real_t *restrict indTerms,
                                        real_t *restrict solution,
                                        uint size)
{
    __m256d aux;
    __m256d *avxSol = (__m256d *)(solution);

    for (uint line = 0; line < size; line++)
    {
        solution[line] = indTerms[line];
        int unrollLimit = floor(line / SIMD_DBL_QTD) * SIMD_DBL_QTD;
        int vectorSize = unrollLimit / SIMD_DBL_QTD;

        __m256d *avxMatrix = (__m256d *)(matrix[line]);

        for (uint column = 0; column < vectorSize; column++)
        {
            aux = _mm256_mul_pd(avxSol[column], avxMatrix[column]);
            solution[line] -= hsum_double_avx(aux);
        }

        for (uint k = unrollLimit; k < line; k++)
            solution[line] -= matrix[line][k] * solution[k];

        solution[line] = solution[line] / matrix[line][line];
    }

    return success;
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
    int vectorSize = size / SIMD_DBL_QTD;
    int vectorLimit = size - (size % SIMD_DBL_QTD);
    __m256d aux;

    for (uint i = 0; i < size; i++)
    {
        __m256d *avxResidual = (__m256d *)(residual[i]);
        for (uint j = 0; j < vectorSize; j++)
        {
            aux = _mm256_mul_pd(avxResidual[j], avxResidual[j]);
            sum += hsum_double_avx(aux);
        }

        for (uint j = vectorLimit; j < size; j++)
            sum += residual[i][j] * residual[i][j];
    }
    *result = sqrt(sum);
    return success;
}

/*!
  \brief Função que vai calcular o resíduo da iteração atual do refinamento
  *
  \param identity Ponteiro para a matriz identidade
  \param matrix Ponteiro para a a solução atual da matriz
  \param solution Ponteiro para a matriz inversaontas
  \param residuals Ponteiro para a matriz de residuos
  \param size Tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcRefinementResidual(real_t **restrict identity,
                                      real_t **restrict matrix,
                                      real_t **restrict solution,
                                      real_t **restrict residuals,
                                      uint size)
{
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
                residuals[i][j] -= hsum_double_avx(aux);
            }

            for (uint l = unrollLimit; l < size; l++)
                residuals[i][j] -= solution[i][l] * matrix[j][l];
        }
    }
    return success;
}

/*!
  \brief Função que vai calcular a nova aproximação do refinamento para a próxima iteração
  *
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param residuals Ponteiro para a matriz de residuos
  \param L Ponteiro para a matriz L
  \param curSol Ponteiro para o vetor de solução atual do refinamento
  \param solution Ponteiro para o vetor de solução da interação anterior
  \param U Ponteiro para a matriz U
  \param size Tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcRefinementNewApproximation(uint *lineSwaps,
                                              real_t **restrict residuals,
                                              real_t **restrict L,
                                              real_t *curSol,
                                              real_t **restrict solution,
                                              real_t **restrict U,
                                              uint size)
{
    for (int i = 0; i < size; i++)
    {
        applyLineSwapsOnArray(lineSwaps, residuals[i], size);

        reverseRetroSubstitution(L, residuals[i], curSol, size);

        retroSubstitution(U, curSol, curSol, size);
        // soma a solução do resíduo com a solução anterior para obter a nova apromixação
        for (int j = 0; j < size; j++)
            solution[i][j] += curSol[j];
    }

    return success;
}
