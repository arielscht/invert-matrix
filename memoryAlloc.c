#include "memoryAlloc.h"

void freeMatrix(real_t **matrix, uint size)
{
    if (!matrix)
        return;

    for (uint i = 0; i < size; i++)
    {
        if (matrix[i])
            free(matrix[i]);
    }
    free(matrix);
}

void freeArray(void *array)
{
    if (!array)
        return;

    free(array);
}

real_t **allocMatrix(uint size)
{
    real_t **matrix = calloc(size, sizeof(real_t *));

    if (!matrix)
        return NULL;

    for (uint i = 0; i < size; i++)
    {
        matrix[i] = calloc(size, sizeof(real_t));

        if (!matrix[i])
        {
            for (uint j = i - 1; j >= 0; j--)
                free(matrix[j]);
            free(matrix);

            return NULL;
        }
    }

    return matrix;
}

uint *allocUintArray(uint size)
{
    uint *array = calloc(size, sizeof(uint));

    if (!array)
        return NULL;
    return array;
}

real_t *allocDoubleArray(uint size)
{
    real_t *array = calloc(size, sizeof(real_t));

    if (!array)
        return NULL;
    return array;
}

void freeMainMemory(real_t **A, real_t **L, real_t **U, real_t **invertedMatrix, uint *lineSwaps, real_t *iterationsNorm, uint size)
{
    freeMatrix(A, size);
    freeMatrix(L, size);
    freeMatrix(U, size);
    freeMatrix(invertedMatrix, size);
    freeArray(lineSwaps);
    freeArray(iterationsNorm);
}