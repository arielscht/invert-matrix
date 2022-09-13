#include <stdio.h>
#include <math.h>
#include <float.h>
// #include <likwid.h>
#include "../methods/methods.h"
#include "../utils/utils.h"
#include "../memoryAlloc/memoryAlloc.h"
#include "../sisUtils/sisUtils.h"

/*!
  \brief Função que vai refinar a matrix inversa
  *
  \param A Ponteiro para a matriz A
  \param L Ponteiro para a matriz L
  \param U Ponteiro para a matriz U
  \param solution Ponteiro para a matriz inversa
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param size Tamanho da matriz
  \param iterations Quantidade de iterações a serem executadas no refinamento
  \param iterationsNorm Ponteiro para o vetor das normas de cada iteração
  \param tTotalRefinement Ponteiro para o Tempo total do refinamento
  \param avgTimeNorm Ponteiro para o tempo total do cálculo da norma
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus refinement(real_t **restrict A,
                          real_t **restrict L,
                          real_t **restrict U,
                          real_t **restrict solution,
                          uint *lineSwaps,
                          uint size,
                          int iterations,
                          real_t *restrict iterationsNorm,
                          real_t *restrict tTotalRefinement,
                          real_t *restrict avgTimeNorm)
{
    FunctionStatus status = success;
    real_t **identity = allocMatrix(size);
    real_t **residuals = allocMatrix(size);
    real_t *curSol = allocDoubleArray(size);
    real_t norm = 0.0;
    real_t auxNormTime = 0;
    real_t auxResidualTime = 0;
    int counter = 1;

    if ((status = verifyRefinementAllocs(identity, residuals, curSol)) == success)
    {
        initIdentityMatrix(identity, size);

        *tTotalRefinement = timestamp();
        while (counter <= iterations)
        {
            // Calcula resíduo
            LIKWID_MARKER_START("OP2");
            auxResidualTime = timestamp();
            calcRefinementResidual(identity, A, solution, residuals, size);
            LIKWID_MARKER_STOP("OP2");
            // Calcula nova aproximação
            calcRefinementNewApproximation(lineSwaps, residuals, L, curSol, solution, U, size);

            auxNormTime = timestamp();
            calcL2Norm(residuals, size, &norm);
            *avgTimeNorm += timestamp() - auxNormTime;

            iterationsNorm[counter - 1] = norm;
            counter++;
        };
        if (status == success)
        {
            *tTotalRefinement = timestamp() - *tTotalRefinement;
            *tTotalRefinement /= counter;
            *avgTimeNorm /= counter;
        }
    }

    freeRefinementMemory(identity, residuals, curSol, size);
    return status;
}

/*!
  \brief Função que aplica a eliminação de gauss na matriz A
  *
  \param A Ponteiro para a matriz A
  \param L Ponteiro para a matriz L
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param size Tamanho da matriz
  \param tTotal Ponteiro para o Tempo total da eliminação de gauss
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus gaussElimination(real_t **restrict A,
                                real_t **restrict L,
                                uint *lineSwaps,
                                uint size,
                                real_t *tTotal)
{
    *tTotal = timestamp();
    real_t mult;

    for (uint line = 0; line < size - 1; line++)
    {
        uint pivotLine = findPivot(A, line, size);
        if (pivotLine != line)
        {
            swapLines(A, line, pivotLine, size);

            real_t aux = lineSwaps[line];
            lineSwaps[line] = lineSwaps[pivotLine];
            lineSwaps[pivotLine] = aux;
            swapLines(L, line, pivotLine, size);
        }

        for (uint auxLine = line + 1; auxLine < size; auxLine++)
        {
            real_t m = A[auxLine][line] / A[line][line];
            A[auxLine][line] = 0.0;
            L[auxLine][line] = m;

            for (uint column = line + 1; column < size; column++)
            {
                mult = A[line][column] * m;
                A[auxLine][column] -= mult;
            }
        }
    }

    *tTotal = timestamp() - *tTotal;
    return success;
}

/*!
  \brief Função que aplica a Fatoração LU em A
  *
  \param A Ponteiro para a matriz A
  \param L Ponteiro para a matriz L
  \param U Ponteiro para a matriz U
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param size Tamanho da matriz
  \param tTotal Ponteiro para o Tempo total da eliminação de gauss
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus factorizationLU(real_t **restrict A,
                               real_t **restrict L,
                               real_t **restrict U,
                               uint *lineSwaps,
                               uint size,
                               real_t *tTotal)
{
    copyMatrix(A, U, size);
    cleanMatrix(L, size);
    gaussElimination(U, L, lineSwaps, size, tTotal);
    setMainDiagonal(L, 1.0, size);

    return success;
}

/*!
  \brief Função que vai obter a inversa da matriz A
  *
  \param A Ponteiro para a matriz A
  \param L Ponteiro para a matriz L
  \param U Ponteiro para a matriz U
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param invertedMatrix Ponteiro para a matriz invertedMatrix
  \param size Tamanho da matriz
  \param tFactorization Ponteiro para o Tempo total da eliminação de gauss
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus reverseMatrix(real_t **restrict A,
                             real_t **restrict L,
                             real_t **restrict U,
                             uint *lineSwaps,
                             real_t **restrict invertedMatrix,
                             uint size,
                             real_t *tFactorization)
{
    FunctionStatus status = success;
    real_t **identity = allocMatrix(size);

    if ((status = verifyReverseMatrixAllocs(identity)) == success)
    {
        initArrayWithIndexes(lineSwaps, size);
        initIdentityMatrix(identity, size);

        LIKWID_MARKER_START("OP1");
        if ((status = factorizationLU(A, L, U, lineSwaps, size, tFactorization)) == success)
        {
            applyLineSwaps(lineSwaps, identity, size);

            for (uint i = 0; i < size; i++)
            {
                reverseRetroSubstitution(L, identity[i], invertedMatrix[i], size);
                retroSubstitution(U, invertedMatrix[i], invertedMatrix[i], size);
            }
        }
        LIKWID_MARKER_STOP("OP1");
    }

    freeReverseMatrixMemory(identity, size);
    return status;
}
