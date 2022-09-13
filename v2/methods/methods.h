#ifndef __METHODS_H__
#define __METHODS_H__
#define ERRO_NORM 1.0e-6
#define MAXIT 1000

#include "../sislin/sislin.h"

FunctionStatus reverseMatrix(real_t **restrict A,
                             real_t **restrict L,
                             real_t **restrict U,
                             uint *lineSwaps,
                             real_t **restrict invertedMatrix,
                             uint size,
                             real_t *tFactorization);

FunctionStatus refinement(real_t **restrict A,
                          real_t **restrict L,
                          real_t **restrict U,
                          real_t **restrict solution,
                          uint *lineSwaps,
                          uint size,
                          int iterations,
                          real_t *restrict iterationsNorm,
                          real_t *restrict tTotalRefinement,
                          real_t *restrict avgTimeNorm);

#endif