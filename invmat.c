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
    char outputFilename[200];
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    int iterations = 0;
    uint size = 0;
    int skipInputFile = 0;

    inputFilename[0] = '\0';
    outputFilename[0] = '\0';

    handleArgs(argc, argv, inputFilename, outputFilename, &iterations, &size);
    handleInput(&inputFile, inputFilename);
    handleOutput(&outputFile, outputFilename);

    if (!size)
    {
        fscanf(inputFile, "%d", &size);
        skipInputFile = 1;
    }

    real_t **A = allocMatrix(size);
    real_t **L = allocMatrix(size);
    real_t **U = allocMatrix(size);
    real_t **invertedMatrix = allocMatrix(size);
    uint *lineSwaps = allocUintArray(size);
    real_t *iterationsNorm = allocDoubleArray(iterations);

    if (!A || !L || !U || !invertedMatrix || !lineSwaps || !iterationsNorm)
    {
        freeMatrix(A, size);
        freeMatrix(L, size);
        freeMatrix(U, size);
        freeMatrix(invertedMatrix, size);
        freeArray(lineSwaps);
        freeArray(iterationsNorm);
        fprintf(stderr, "Memory allocation error!\n");

        return 1;
    }

    real_t totalTimeFactorization;
    real_t averageTimeRefinement;
    // real_t totalTimeResidual;

    if (skipInputFile)
        readMatrixFromFile(A, size, inputFile);
    else
        initRandomMatrix(A, generico, COEF_MAX, size);

    reverseMatrix(A, L, U, lineSwaps, invertedMatrix, size, &totalTimeFactorization);
    refinement(A, L, U, invertedMatrix, lineSwaps, size, iterations, iterationsNorm, &averageTimeRefinement);

    printFinalOutput(outputFile, iterationsNorm, totalTimeFactorization, averageTimeRefinement, 0.0, size, A, invertedMatrix, iterations);

    freeMatrix(A, size);
    freeMatrix(L, size);
    freeMatrix(U, size);
    freeMatrix(invertedMatrix, size);
    freeArray(lineSwaps);
    freeArray(iterationsNorm);

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}