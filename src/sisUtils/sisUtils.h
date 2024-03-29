#ifndef __SIS_UTILS_H__
#define __SIS_UTILS_H__

#include "../utils/utils.h"
#include "../sislin/sislin.h"

uint findPivot(real_t **A, uint curColumn, uint size);

void swapLines(real_t **matrix, uint line1, uint line2);

FunctionStatus retroSubstitution(SistLinear_t *SL, real_t *solution);

FunctionStatus reverseRetroSubstitution(SistLinear_t *SL, real_t *solution);

FunctionStatus calcL2Norm(real_t **residual, uint size, real_t *result);

FunctionStatus calcResidual(SistLinear_t *SL, real_t *solution, real_t *residual);

FunctionStatus calcRefinementResidual(real_t **identity,
                                      SistLinear_t *auxSL,
                                      real_t **solution,
                                      real_t *curSol,
                                      real_t **residuals,
                                      uint size);

FunctionStatus calcRefinementNewApproximation(uint *lineSwaps,
                                              real_t **residuals,
                                              real_t **L,
                                              SistLinear_t *auxSL,
                                              real_t *curSol,
                                              real_t **solution,
                                              real_t **U,
                                              uint size);
#endif