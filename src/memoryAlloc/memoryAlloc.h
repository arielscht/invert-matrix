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

FunctionStatus verifyMainAllocs(real_t **A,
                                real_t **L,
                                real_t **U,
                                real_t **invertedMatrix,
                                uint *lineSwaps,
                                real_t *iterationsNorm);

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

FunctionStatus verifyRefinementAllocs(real_t **identity,
                                      real_t **residuals,
                                      real_t *curSol);

void freeRefinementMemory(real_t **identity,
                          real_t **residuals,
                          real_t *curSol,
                          uint size);

FunctionStatus verifyReverseMatrixAllocs(SistLinear_t *auxSL,
                                         real_t **identity);

void freeReverseMatrixMemory(SistLinear_t *auxSL,
                             real_t **identity,
                             uint size);

#endif