#ifndef __MEMORY_ALLOC_H__
#define __MEMORY_ALLOC_H__

#include <stdio.h>
#include "utils.h"

void freeMatrix(real_t **matrix, uint size);

void freeArray(void *array);

real_t **allocMatrix(uint size);

int *allocIntArray(uint size);

uint *allocUintArray(uint size);

real_t *allocDoubleArray(uint size);

void freeMainMemory(
    real_t **A,
    real_t **L,
    real_t **U,
    real_t **invertedMatrix,
    uint *lineSwaps,
    real_t *iterationsNorm,
    uint size,
    FILE *inputFile,
    FILE *outputFile);

FunctionStatus verifyMainAllocs(real_t **A,
                                real_t **L,
                                real_t **U,
                                real_t **invertedMatrix,
                                uint *lineSwaps,
                                real_t *iterationsNorm);

#endif