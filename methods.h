#ifndef __METHODS_H__
#define __METHODS_H__
#define ERRO_COMPARISON 1.0e-6 // Error tolerance for comparing two real type values

#include "sislin.h"

int gaussElimination(SistLinear_t *SL, real_t *solution, real_t *tTotal);

#endif