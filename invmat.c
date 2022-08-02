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

    // real_t **A;
    uint size;

    // handleArgs(argc, argv, inputFilename, outputFilename, &iterations);
    // handleInput("input.txt", &A, &size);
    // printf("OUTSIDE:\n");
    // printMatrix(A, size);
    // freeMatrix(A, size);
    // return 0;

    scanf("%u", &size);

    real_t **A = allocMatrix(size);
    real_t **L = allocMatrix(size);
    real_t **U = allocMatrix(size);
    uint *lineSwaps = calloc(size, sizeof(uint));
    real_t **invertedMatrix = allocMatrix(size);
    real_t tTotal;

    readMatrix(A, size);

    reverseMatrix(A, L, U, lineSwaps, invertedMatrix, size, &tTotal);

    printf("\nORIGINAL MATRIX: \n");
    printMatrix(A, size);
    printf("\nINVERTED MATRIX: \n");
    printMatrix(invertedMatrix, size);

    printf("\nREFINING...\n");
    refinement(A, L, U, invertedMatrix, lineSwaps, size, 10, 1.0e-6);

    printf("\nORIGINAL MATRIX: \n");
    printMatrix(A, size);
    printf("\nINVERTED MATRIX: \n");
    printMatrix(invertedMatrix, size);

    freeMatrix(A, size);
    freeMatrix(L, size);
    freeMatrix(U, size);
    freeMatrix(invertedMatrix, size);
    free(lineSwaps);
    return 0;
}