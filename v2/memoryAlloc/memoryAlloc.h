#ifndef __MEMORY_ALLOC_H__
#define __MEMORY_ALLOC_H__

#include <stdio.h>
#include "../utils/utils.h"
#include "../sislin/sislin.h"

void freeMatrix(real_t **matrix, uint size);

void freeArray(void *array);

real_t **allocMatrix(uint size);

int *allocIntArray(uint size);

uint *allocUintArray(uint size);

real_t *allocDoubleArray(uint size);

FunctionStatus verifyMainAllocs(real_t **restrict A,
                                real_t **restrict L,
                                real_t **restrict U,
                                real_t **restrict invertedMatrix,
                                uint *lineSwaps,
                                real_t *iterationsNorm);

void freeMainMemory(
    real_t **restrict A,
    real_t **restrict L,
    real_t **restrict U,
    real_t **restrict invertedMatrix,
    uint *lineSwaps,
    real_t *iterationsNorm,
    uint size,
    FILE *restrict inputFile,
    FILE *restrict outputFile);

FunctionStatus verifyRefinementAllocs(real_t **restrict identity,
                                      real_t **restrict residuals,
                                      real_t *curSol);

void freeRefinementMemory(real_t **restrict identity,
                          real_t **restrict residuals,
                          real_t *curSol,
                          uint size);

FunctionStatus verifyReverseMatrixAllocs(real_t **identity);

void freeReverseMatrixMemory(real_t **identity,
                             uint size);

#endif