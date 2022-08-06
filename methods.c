#include <stdio.h>
#include "methods.h"
#include "utils.h"
#include "math.h"
#include "float.h"

uint findPivot(real_t **A, uint curColumn, uint size)
{
    uint pivot = curColumn;

    for (uint line = curColumn + 1; line < size; line++)
        if ((A[line][curColumn] - A[pivot][curColumn]) > ERRO_COMPARISON)
            pivot = line;

    return pivot;
}

void swapLines(real_t **matrix, uint line1, uint line2)
{
    real_t *auxLine = matrix[line1];
    matrix[line1] = matrix[line2];
    matrix[line2] = auxLine;
}

FunctionStatus retroSubstitution(SistLinear_t *SL, real_t *solution)
{
    uint size = SL->n;
    FunctionStatus status = success;
    real_t mult;

    for (uint line = size; line >= 1; line--)
    {
        // variable used due to uint never being less than 0
        uint actualLine = line - 1;
        solution[actualLine] = SL->b[actualLine];
        for (uint column = size - 1; column > actualLine; column--)
        {
            if (status = multiplyDouble(&mult, SL->A[actualLine][column], solution[column]) != success)
                return status;
            solution[actualLine] -= mult;
        }
        if (status = divideDouble(&solution[actualLine], solution[actualLine], SL->A[actualLine][actualLine]) != success)
            return status;
    }
    return status;
}

FunctionStatus reverseRetroSubstitution(SistLinear_t *SL, real_t *solution)
{
    uint size = SL->n;
    FunctionStatus status = success;
    real_t mult;

    for (int line = 0; line < size; line++)
    {
        solution[line] = SL->b[line];
        for (int column = 0; column < line; column++)
        {
            if (status = multiplyDouble(&mult, SL->A[line][column], solution[column]) != success)
                return status;
            solution[line] -= mult;
        }
        if (status = divideDouble(&solution[line], solution[line], SL->A[line][line]) != success)
            return status;
    }

    return status;
}

FunctionStatus calcL2Norm(real_t **residual, uint size, real_t *result)
{
    real_t sum = 0.0;
    FunctionStatus status = success;
    real_t mult;

    for (uint i = 0; i < size; i++)
        for (uint j = 0; j < size; j++)
        {
            if (status = multiplyDouble(&mult, residual[i][j], residual[i][j]) != success)
                return status;
            sum += mult;
        }
    *result = sqrt(sum);
    return status;
}

FunctionStatus calcResidual(SistLinear_t *SL, real_t *solution, real_t *residual)
{
    uint size = SL->n;
    FunctionStatus status = success;
    real_t mult;

    for (size_t i = 0; i < size; i++)
    {
        residual[i] = SL->b[i];
        for (size_t j = 0; j < size; j++)
        {
            if (status = multiplyDouble(&mult, solution[j], SL->A[i][j]) != success)
                return status;
            residual[i] -= mult;
        }
    }

    return status;
}

FunctionStatus refinement(real_t **A,
                          real_t **L,
                          real_t **U,
                          real_t **solution,
                          uint *lineSwaps,
                          uint size,
                          int iterations,
                          FILE *outputFile,
                          real_t *tTotal)
{
    *tTotal = timestamp();
    FunctionStatus status = success;
    real_t **residuals = allocMatrix(size);
    real_t *curSol = allocDoubleArray(size);
    real_t norm = 0.0;
    int counter = 1;

    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    copyMatrix(A, auxSL->A, size);

    real_t **identity = allocMatrix(size);
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
        fprintf(outputFile, "# iter %d: <||%.15g||>\n", counter, norm); //
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
