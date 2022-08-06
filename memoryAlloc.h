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

#endif