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
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;
    int iterations = 0;
    uint size = 0;
    int skipInputFile = 0;
    FunctionStatus status = success;
    real_t **A = NULL;
    real_t **L = NULL;
    real_t **U = NULL;
    real_t **invertedMatrix = NULL;
    uint *lineSwaps = NULL;
    real_t *iterationsNorm = NULL;
    real_t totalTimeFactorization = 0;
    real_t averageTimeRefinement = 0;
    real_t averageTimeResidual = 0;

    inputFilename[0] = '\0';
    outputFilename[0] = '\0';

    handleArgs(argc, argv, inputFilename, outputFilename, &iterations, &size);

    if ((status = handleMainInput(&size, &inputFile, inputFilename, &skipInputFile)) == success)
    {
        A = allocMatrix(size);
        L = allocMatrix(size);
        U = allocMatrix(size);
        invertedMatrix = allocMatrix(size);
        lineSwaps = allocUintArray(size);
        iterationsNorm = allocDoubleArray(iterations);

        if ((status = verifyMainAllocs(A, L, U, invertedMatrix, lineSwaps, iterationsNorm)) == success &&
            (status = initializeMainMatrix(skipInputFile, A, size, inputFile)) == success &&
            (status = reverseMatrix(A, L, U, lineSwaps, invertedMatrix, size, &totalTimeFactorization)) == success &&
            (status = refinement(A, L, U, invertedMatrix, lineSwaps, size, iterations, iterationsNorm, &averageTimeRefinement, &averageTimeResidual)) == success &&
            (status = handleFile(&outputFile, outputFilename, "w")) == success)
            printFinalOutput(outputFile, iterationsNorm, totalTimeFactorization, averageTimeRefinement, averageTimeResidual, size, invertedMatrix, iterations);
    }

    if (status != success)
        handleErrorsException(status);

    freeMainMemory(A, L, U, invertedMatrix, lineSwaps, iterationsNorm, size, inputFile, outputFile);

    return status;
}