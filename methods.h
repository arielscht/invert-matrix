#ifndef __METHODS_H__
#define __METHODS_H__
#define ERRO_COMPARISON 1.0e-6 // Error tolerance for comparing two real type values

#include "sislin.h"

int factorizationLU(SistLinear_t *SL,
                    real_t **L,
                    real_t **U,
                    real_t **identity);

int reverseMatrix(SistLinear_t *SL,
                  real_t **L,
                  real_t **U,
                  real_t **identity,
                  real_t *tTotal);

#endif