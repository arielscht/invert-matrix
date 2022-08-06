#ifndef __SIS_UTILS_H__
#define __SIS_UTILS_H__

#include "utils.h"
#include "sislin.h"

uint findPivot(real_t **A, uint curColumn, uint size);

void swapLines(real_t **matrix, uint line1, uint line2);

FunctionStatus retroSubstitution(SistLinear_t *SL, real_t *solution);

FunctionStatus reverseRetroSubstitution(SistLinear_t *SL, real_t *solution);

FunctionStatus calcL2Norm(real_t **residual, uint size, real_t *result);

FunctionStatus calcResidual(SistLinear_t *SL, real_t *solution, real_t *residual);

#endif