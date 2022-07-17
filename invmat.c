#include <stdio.h>
#include <stdlib.h>

#include "sislin.h"
#include "methods.h"
#include "utils.h"

#define SYSTEM_SIZE 7

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
    real_t **invertedMatrix = allocMatrix(SL->n);
    real_t **multiplication = allocMatrix(SL->n);

    prnSisLin(SL);

    reverseMatrix(SL, L, U, identity, invertedMatrix, &tTotal);

    printMatrix(invertedMatrix, SYSTEM_SIZE);

    multiplyMatrix(multiplication, SL->A, invertedMatrix, SYSTEM_SIZE);
    printf("\n");
    printMatrix(multiplication, SYSTEM_SIZE);

    liberaSisLin(SL);
    freeMatrix(L, SYSTEM_SIZE);
    freeMatrix(U, SYSTEM_SIZE);
    freeMatrix(identity, SYSTEM_SIZE);
    freeMatrix(invertedMatrix, SYSTEM_SIZE);
    freeMatrix(multiplication, SYSTEM_SIZE);
    return 0;
}