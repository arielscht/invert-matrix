#ifndef __METHODS_H__
#define __METHODS_H__
#define ERRO_NORM 1.0e-6
#define MAXIT 1000

#include "../sislin/sislin.h"

FunctionStatus reverseMatrix(real_t **A,
                             real_t **L,
                             real_t **U,
                             uint *lineSwaps,
                             real_t **invertedMatrix,
                             uint size,
                             real_t *tFactorization,
                             real_t *tFirstSolution);

FunctionStatus refinement(real_t **A,
                          real_t **L,
                          real_t **U,
                          real_t **solution,
                          uint *lineSwaps,
                          uint size,
                          int iterations,
                          real_t *iterationsNorm,
                          real_t *tTotalRefinement,
                          real_t *avgTimeNorm,
                          real_t *avgTimeResidual);

#endif