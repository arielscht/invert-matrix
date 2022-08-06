#include <stdio.h>
#include <stdlib.h>

#include "sislin.h"
#include "methods.h"
#include "utils.h"
#include "interface.h"
#include "memoryAlloc.h"

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
    FunctionStatus status = success;
    inputFilename[0] = '\0';
    outputFilename[0] = '\0';

    handleArgs(argc, argv, inputFilename, outputFilename, &iterations, &size);

    if (status = handleInput(&inputFile, inputFilename) != success)
    {
        fprintf(stderr, "Fail to handle the input file");
        return status;
    }
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
        freeMainMemory(A, L, U, invertedMatrix, lineSwaps, iterationsNorm, size);
        fclose(inputFile);
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

    if (status = handleOutput(&outputFile, outputFilename) != success)
    {
        fprintf(stderr, "Fail to handle the output file");
        return status;
    }

    printFinalOutput(outputFile, iterationsNorm, totalTimeFactorization, averageTimeRefinement, 0.0, size, invertedMatrix, iterations);

    freeMainMemory(A, L, U, invertedMatrix, lineSwaps, iterationsNorm, size);

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}