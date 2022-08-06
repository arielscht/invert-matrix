#include <stdio.h>
#include <float.h>
#include <math.h>
#include "sisUtils.h"

uint findPivot(real_t **A, uint curColumn, uint size)
{
    uint pivot = curColumn;

    for (uint line = curColumn + 1; line < size; line++)
        if ((A[line][curColumn] - A[pivot][curColumn]) > DBL_EPSILON)
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
