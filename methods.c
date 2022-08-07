#include <stdio.h>
#include <math.h>
#include <float.h>
#include "methods.h"
#include "utils.h"
#include "memoryAlloc.h"
#include "sisUtils.h"

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
  \param tTotalResidual Ponteiro para o tempo total do cálculo da norma
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus refinement(real_t **A,
                          real_t **L,
                          real_t **U,
                          real_t **solution,
                          uint *lineSwaps,
                          uint size,
                          int iterations,
                          real_t *iterationsNorm,
                          real_t *tTotalRefinement,
                          real_t *tTotalResidual)
{
    FunctionStatus status = success;
    real_t **identity = allocMatrix(size);
    real_t **residuals = allocMatrix(size);
    real_t *curSol = allocDoubleArray(size);
    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    real_t norm = 0.0;
    real_t auxTime = 0;
    int counter = 1;

    if ((status = verifyRefinementAllocs(identity, residuals, curSol, auxSL)) == success)
    {
        copyMatrix(A, auxSL->A, size);
        initIdentityMatrix(identity, size);

        *tTotalRefinement = timestamp();
        while (counter <= iterations && status == success)
        {
            // Calcula resíduo
            copyMatrix(A, auxSL->A, size);
            if ((status = calcRefinementResidual(identity, auxSL, solution, curSol, residuals, size)) != success)
                continue;
            // Calcula nova aproximação
            if ((status = calcRefinementNewApproximation(lineSwaps, residuals, L, auxSL, curSol, solution, U, size)) != success)
                continue;

            auxTime = timestamp();
            if ((status = calcL2Norm(residuals, size, &norm)) != success)
                continue;
            *tTotalResidual += timestamp() - auxTime;

            iterationsNorm[counter - 1] = norm;
            counter++;
        };
        if (status == success)
        {
            *tTotalRefinement = timestamp() - *tTotalRefinement;
            *tTotalRefinement /= counter;
            *tTotalResidual /= counter;
        }
    }

    freeRefinementMemory(identity, residuals, curSol, auxSL, size);
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
FunctionStatus gaussElimination(real_t **A,
                                real_t **L,
                                uint *lineSwaps,
                                uint size,
                                real_t *tTotal)
{
    *tTotal = timestamp();
    FunctionStatus status = success;
    real_t mult;

    for (uint line = 0; line < size - 1; line++)
    {
        uint pivotLine = findPivot(A, line, size);
        if (pivotLine != line)
        {
            swapLines(A, line, pivotLine);

            real_t aux = lineSwaps[line];
            lineSwaps[line] = lineSwaps[pivotLine];
            lineSwaps[pivotLine] = aux;
            if (L != NULL)
                swapLines(L, line, pivotLine);
        }

        for (uint auxLine = line + 1; auxLine < size; auxLine++)
        {
            real_t m;
            if ((status = divideDouble(&m, A[auxLine][line], A[line][line])) != success)
                return status;
            A[auxLine][line] = 0.0;

            if (L != NULL)
                L[auxLine][line] = m;

            for (uint column = line + 1; column < size; column++)
            {
                if ((status = multiplyDouble(&mult, A[line][column], m)) != success)
                    return status;
                A[auxLine][column] -= mult;
            }
        }
    }

    *tTotal = timestamp() - *tTotal;
    return status;
}

/*!
  \brief Função que aplica a Fatoração LU em A
  *
  \param A Ponteiro para a matriz A
  \param L Ponteiro para a matriz L
  \param L Ponteiro para a matriz U
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param size Tamanho da matriz
  \param tTotal Ponteiro para o Tempo total da eliminação de gauss
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus factorizationLU(real_t **A,
                               real_t **L,
                               real_t **U,
                               uint *lineSwaps,
                               uint size,
                               real_t *tTotal)
{
    FunctionStatus status = success;

    copyMatrix(A, U, size);
    cleanMatrix(L, size);
    if ((status = gaussElimination(U, L, lineSwaps, size, tTotal)) != success)
        return status;
    setMainDiagonal(L, 1.0, size);

    return status;
}

/*!
  \brief Função que vai obter a inversa da matriz A
  *
  \param A Ponteiro para a matriz A
  \param L Ponteiro para a matriz L
  \param L Ponteiro para a matriz U
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param invertedMatrix Ponteiro para a matriz invertedMatrix
  \param size Tamanho da matriz
  \param tTotal Ponteiro para o Tempo total da eliminação de gauss
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus reverseMatrix(real_t **A,
                             real_t **L,
                             real_t **U,
                             uint *lineSwaps,
                             real_t **invertedMatrix,
                             uint size,
                             real_t *tTotal)
{
    FunctionStatus status = success;
    real_t det;
    if ((status = calcDet(&det, A, size)) != success || fabs(det) < DBL_EPSILON)
    {
        status = status != success ? status : nonInvertibleErr;
        return status;
    }

    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    real_t *sol = allocDoubleArray(size);
    real_t **identity = allocMatrix(size);

    if ((status = verifyReverseMatrixAllocs(auxSL, sol, identity)) == success)
    {
        initArrayWithIndexes(lineSwaps, size);
        initIdentityMatrix(identity, size);

        if ((status = factorizationLU(A, L, U, lineSwaps, size, tTotal)) == success)
        {
            applyLineSwaps(lineSwaps, identity, size);

            for (uint i = 0; i < size && status == success; i++)
            {
                copyColumnToArray(identity, auxSL->b, i, size);
                copyMatrix(L, auxSL->A, size);

                if ((status = reverseRetroSubstitution(auxSL, sol)) != success)
                    continue;

                copyMatrix(U, auxSL->A, size);
                copyArray(sol, auxSL->b, size);

                if ((status = retroSubstitution(auxSL, sol)) != success)
                    continue;

                for (uint j = 0; j < size; j++)
                    invertedMatrix[j][i] = sol[j];
            }
        }
    }

    freeReverseMatrixMemory(auxSL, sol, identity, size);
    return status;
}
