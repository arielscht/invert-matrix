#ifndef __INTERFACE__
#define __INTERFACE__

#include "utils.h"

void handleArgs(int argc,
                char *argv[],
                char *inputFilename,
                char *outputFilename,
                int *iterations,
                uint *size);

FunctionStatus handleInput(
    FILE **inputFile,
    char *filename);

FunctionStatus handleOutput(
    FILE **outputFile,
    char *filename);

void printFinalOutput(FILE *outputFile,
                      real_t *iterationsNorm,
                      real_t totalTimeFactorization,
                      real_t averageTimeRefinement,
                      real_t averageTimeNorm,
                      uint size,
                      real_t **invertedMatrix,
                      uint iterations);

void handleErrorsException(FunctionStatus status);

#endif