#include <stdio.h>
#include <math.h>
#include <float.h>
#include <likwid.h>
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
                          real_t *avgTimeNorm,
                          real_t *avgTimeResidual)
{
    FunctionStatus status = success;
    real_t **identity = allocMatrix(size);
    real_t **residuals = allocMatrix(size);
    real_t *curSol = allocDoubleArray(size);
    real_t *indTerms = allocDoubleArray(size);
    real_t norm = 0.0;
    real_t auxNormTime = 0;
    real_t auxResidualTime = 0;
    int counter = 1;

    if ((status = verifyRefinementAllocs(identity, residuals, curSol, indTerms)) == success)
    {
        initIdentityMatrix(identity, size);

        *tTotalRefinement = timestamp();
        while (counter <= iterations && status == success)
        {
            // Calcula resíduo
            LIKWID_MARKER_START("OP2");
            auxResidualTime = timestamp();
            if ((status = calcRefinementResidual(identity, A, indTerms, solution, curSol, residuals, size)) != success)
                continue;
            *avgTimeResidual += timestamp() - auxResidualTime;
            LIKWID_MARKER_STOP("OP2");
            // Calcula nova aproximação
            if ((status = calcRefinementNewApproximation(lineSwaps, residuals, L, curSol, solution, U, size)) != success)
                continue;

            auxNormTime = timestamp();
            if ((status = calcL2Norm(residuals, size, &norm)) != success)
                continue;
            *avgTimeNorm += timestamp() - auxNormTime;

            iterationsNorm[counter - 1] = norm;
            counter++;
        };
        if (status == success)
        {
            *tTotalRefinement = timestamp() - *tTotalRefinement;
            *tTotalRefinement /= counter;
            *avgTimeNorm /= counter;
            *avgTimeResidual /= counter;
        }
    }

    freeRefinementMemory(identity, residuals, curSol, indTerms, size);
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
            swapLines(A, line, pivotLine, size);

            real_t aux = lineSwaps[line];
            lineSwaps[line] = lineSwaps[pivotLine];
            lineSwaps[pivotLine] = aux;
            swapLines(L, line, pivotLine, size);
        }

        for (uint auxLine = line + 1; auxLine < size; auxLine++)
        {
            real_t m;
            if ((status = divideDouble(&m, A[auxLine][line], A[line][line])) != success)
                return status;
            A[auxLine][line] = 0.0;
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
                             real_t *tFactorization,
                             real_t *tFirstSolution)
{
    FunctionStatus status = success;
    // real_t det;
    // if ((status = calcDet(&det, A, size)) != success || fabs(det) < DBL_EPSILON)
    // {
    //     status = status != success ? status : nonInvertibleErr;
    //     return status;
    // }

    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    real_t *sol = allocDoubleArray(size);
    real_t **identity = allocMatrix(size);

    if ((status = verifyReverseMatrixAllocs(auxSL, sol, identity)) == success)
    {
        initArrayWithIndexes(lineSwaps, size);
        initIdentityMatrix(identity, size);

        LIKWID_MARKER_START("OP1");
        *tFirstSolution = timestamp();
        if ((status = factorizationLU(A, L, U, lineSwaps, size, tFactorization)) == success)
        {
            applyLineSwaps(lineSwaps, identity, size);

            for (uint i = 0; i < size && status == success; i++)
            {

                if ((status = reverseRetroSubstitution(L, identity[i], sol, size)) != success)
                    continue;

                if ((status = retroSubstitution(U, sol, sol, size)) != success)
                    continue;

                copyArray(sol, invertedMatrix[i], size);
            }
        }
        *tFirstSolution = timestamp() - *tFirstSolution;
        LIKWID_MARKER_STOP("OP1");
    }

    freeReverseMatrixMemory(auxSL, sol, identity, size);
    return status;
}
