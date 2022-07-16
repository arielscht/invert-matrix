#include <stdio.h>
#include <stdlib.h>

#include "sislin.h"
#include "methods.h"
#include "utils.h"

#define SYSTEM_SIZE 5

int main()
{
    srand(20221);
    SistLinear_t *SL;
    real_t tTotal;

    SL = alocaSisLin(SYSTEM_SIZE, pontPont);
    iniSisLin(SL, generico, COEF_MAX);

    real_t **L = allocMatrix(SL->n);
    real_t **U = allocMatrix(SL->n);
    real_t **identity = allocMatrix(SL->n);

    factorizationLU(SL, L, U, identity, &tTotal);

    liberaSisLin(SL);
    freeMatrix(L, SYSTEM_SIZE);
    freeMatrix(U, SYSTEM_SIZE);
    freeMatrix(identity, SYSTEM_SIZE);
    return 0;
}