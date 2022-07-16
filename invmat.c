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
    real_t *sol = calloc(SYSTEM_SIZE, sizeof(real_t));
    real_t tTotal;

    SL = alocaSisLin(SYSTEM_SIZE, pontPont);
    iniSisLin(SL, diagDominante, COEF_MAX);

    printf("BEFORE GAUSS: \n");
    prnSisLin(SL);
    gaussElimination(SL, sol, &tTotal);
    printf("AFTER GAUSS: \n");
    prnSisLin(SL);
    prnVetor(sol, SYSTEM_SIZE);

    liberaSisLin(SL);
    free(sol);
    return 0;
}