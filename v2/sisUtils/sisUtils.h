#ifndef __SIS_UTILS_H__
#define __SIS_UTILS_H__

#include "../utils/utils.h"
#include "../sislin/sislin.h"

uint findPivot(real_t **A, uint curColumn, uint size);

void swapLines(real_t **matrix, uint line1, uint line2, uint size);

FunctionStatus retroSubstitution(real_t **matrix,
                                 real_t *indTerms,
                                 real_t *solution,
                                 uint size);

FunctionStatus reverseRetroSubstitution(real_t **matrix,
                                        real_t *indTerms,
                                        real_t *solution,
                                        uint size);

FunctionStatus calcL2Norm(real_t **residual, uint size, real_t *result);

FunctionStatus calcRefinementResidual(real_t **identity,
                                      real_t **matrix,
                                      real_t **solution,
                                      real_t **residuals,
                                      uint size);

FunctionStatus calcRefinementNewApproximation(uint *lineSwaps,
                                              real_t **residuals,
                                              real_t **L,
                                              real_t *curSol,
                                              real_t **solution,
                                              real_t **U,
                                              uint size);
#endif