#include <stdio.h>
#include <math.h>
#include <float.h>
#include "methods.h"
#include "utils.h"
#include "memoryAlloc.h"
#include "sisUtils.h"

FunctionStatus refinement(real_t **A,
                          real_t **L,
                          real_t **U,
                          real_t **solution,
                          uint *lineSwaps,
                          uint size,
                          int iterations,
                          real_t *iterationsNorm,
                          real_t *tTotal)
{
    *tTotal = timestamp();
    FunctionStatus status = success;
    real_t **identity = allocMatrix(size);
    real_t **residuals = allocMatrix(size);
    real_t *curSol = allocDoubleArray(size);
    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    real_t norm = 0.0;
    int counter = 1;

    if (!identity || !residuals || !curSol || !auxSL)
    {
        freeMatrix(identity, size);
        freeMatrix(residuals, size);
        freeArray(curSol);
        liberaSisLin(auxSL);

        status = allocErr;
        return status;
    }

    copyMatrix(A, auxSL->A, size);
    initIdentityMatrix(identity, size);

    while (counter <= iterations)
    {
        // Calcula resíduo e norma
        copyMatrix(A, auxSL->A, size);
        for (int i = 0; i < size; i++)
        {
            copyColumnToArray(identity, auxSL->b, i, size);
            copyColumnToArray(solution, curSol, i, size);

            if (status = calcResidual(auxSL, curSol, residuals[i]) != success)
                return status;
        }

        // Calcula nova aproximação
        for (int i = 0; i < size; i++)
        {
            applyLineSwapsOnArray(lineSwaps, residuals[i], size);
            copyArray(residuals[i], auxSL->b, size);
            copyMatrix(L, auxSL->A, size);

            if (status = reverseRetroSubstitution(auxSL, curSol) != success)
                return status;

            copyMatrix(U, auxSL->A, size);
            copyArray(curSol, auxSL->b, size);

            if (status = retroSubstitution(auxSL, curSol) != success)
                return status;
            // soma a solução do resíduo com a solução anterior para obter a nova apromixação
            for (int j = 0; j < size; j++)
                solution[j][i] += curSol[j];
        }

        if (status = calcL2Norm(residuals, size, &norm) != success)
            return status;
        iterationsNorm[counter - 1] = norm;
        counter++;
    };

    *tTotal = timestamp() - *tTotal;
    *tTotal /= counter;

    freeMatrix(identity, size);
    freeMatrix(residuals, size);
    free(curSol);
    liberaSisLin(auxSL);
    return status;
}

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
            if (status = divideDouble(&m, A[auxLine][line], A[line][line]) != success)
                return status;
            A[auxLine][line] = 0.0;

            if (L != NULL)
                L[auxLine][line] = m;

            for (uint column = line + 1; column < size; column++)
            {
                if (status = multiplyDouble(&mult, A[line][column], m) != success)
                    return status;
                A[auxLine][column] -= mult;
            }
        }
    }

    *tTotal = timestamp() - *tTotal;
    return status;
}

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
    if (status = gaussElimination(U, L, lineSwaps, size, tTotal) != success)
        return status;
    setMainDiagonal(L, 1.0, size);

    return status;
}

FunctionStatus reverseMatrix(real_t **A,
                             real_t **L,
                             real_t **U,
                             uint *lineSwaps,
                             real_t **invertedMatrix,
                             uint size,
                             real_t *tTotal)
{
    FunctionStatus status = success;
    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    real_t *sol = allocDoubleArray(size);
    real_t **identity = allocMatrix(size);
    real_t det;

    if (!auxSL || !sol || !identity)
    {
        liberaSisLin(auxSL);
        freeArray(sol);
        freeMatrix(identity, size);

        status = allocErr;
        return status;
    }

    initArrayWithIndexes(lineSwaps, size);
    initIdentityMatrix(identity, size);

    if (status = factorizationLU(A, L, U, lineSwaps, size, tTotal) != success)
        return status;
    if (status = detTriangularMatrix(&det, U, size) != success)
        return status;
    if (fabs(det) < DBL_EPSILON)
    {
        status = nonInvertibleErr;
        return status;
    }

    applyLineSwaps(lineSwaps, identity, size);

    for (uint i = 0; i < size; i++)
    {
        copyColumnToArray(identity, auxSL->b, i, size);
        copyMatrix(L, auxSL->A, size);

        if (status = reverseRetroSubstitution(auxSL, sol) != success)
            return status;

        copyMatrix(U, auxSL->A, size);
        copyArray(sol, auxSL->b, size);

        if (status = retroSubstitution(auxSL, sol) != success)
            return status;

        for (uint j = 0; j < size; j++)
            invertedMatrix[j][i] = sol[j];
    }

    liberaSisLin(auxSL);
    free(sol);
    freeMatrix(identity, size);
    return status;
}
