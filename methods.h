#ifndef __METHODS_H__
#define __METHODS_H__
#define ERRO_COMPARISON 1.0e-6 // Error tolerance for comparing two real type values
#define ERRO_NORM 1.0e-6
#define MAXIT 1000

#include "sislin.h"

int reverseMatrix(real_t **A,
                  real_t **L,
                  real_t **U,
                  uint *lineSwaps,
                  real_t **invertedMatrix,
                  uint size,
                  real_t *tTotal);

void refinement(real_t **A,
                real_t **L,
                real_t **U,
                real_t **solution,
                uint *lineSwaps,
                uint size,
                int iterations,
                real_t erro);

#endif