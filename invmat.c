#include <stdio.h>
#include <stdlib.h>

#include "sislin.h"
#include "methods.h"
#include "utils.h"
#include "interface.h"

int main(int argc, char *argv[])
{
    srand(20221);
    char inputFilename[200];
    char *outputFilename;
    int iterations;

    real_t **A;
    uint size;

    handleArgs(argc, argv, inputFilename, outputFilename, &iterations);
    handleInput(inputFilename, A, &size);
    printf("OUTSIDE:\n");
    printMatrix(A, size);
    return 0;

    // scanf("%u", &size);

    // // real_t **A = allocMatrix(size);
    // real_t **L = allocMatrix(size);
    // real_t **U = allocMatrix(size);
    // real_t **identity = allocMatrix(size);
    // real_t **invertedMatrix = allocMatrix(size);
    // real_t tTotal;

    // readMatrix(A, size);

    // reverseMatrix(A, L, U, identity, invertedMatrix, size, &tTotal);

    // printMatrix(invertedMatrix, size);

    // freeMatrix(L, size);
    // freeMatrix(U, size);
    // freeMatrix(identity, size);
    // freeMatrix(invertedMatrix, size);
    return 0;
}