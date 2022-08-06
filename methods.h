#ifndef __METHODS_H__
#define __METHODS_H__
#define ERRO_COMPARISON 1.0e-6 // Error tolerance for comparing two real type values
#define ERRO_NORM 1.0e-6
#define MAXIT 1000

#include "sislin.h"

FunctionStatus reverseMatrix(real_t **A,
                             real_t **L,
                             real_t **U,
                             uint *lineSwaps,
                             real_t **invertedMatrix,
                             uint size,
                             real_t *tTotal);

FunctionStatus refinement(real_t **A,
                          real_t **L,
                          real_t **U,
                          real_t **solution,
                          uint *lineSwaps,
                          uint size,
                          int iterations,
                          FILE *outputFile,
                          real_t *tTotal);

#endif