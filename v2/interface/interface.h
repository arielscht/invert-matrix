#ifndef __INTERFACE__
#define __INTERFACE__

#include "../utils/utils.h"

void handleArgs(int argc,
                char *restrict argv[],
                char *restrict inputFilename,
                char *restrict outputFilename,
                int *iterations,
                uint *size);

FunctionStatus handleFile(FILE **inputFile, char *restrict filename, char *restrict fileMode);

void printFinalOutput(FILE *outputFile,
                      real_t *iterationsNorm,
                      real_t totalTimeFactorization,
                      real_t averageTimeRefinement,
                      real_t averageTimeNorm,
                      uint size,
                      real_t **invertedMatrix,
                      uint iterations);

FunctionStatus initializeMainMatrix(int skipInputFile,
                                    real_t **A,
                                    uint size,
                                    FILE *inputFile);

FunctionStatus readMainInput(uint *size,
                             FILE **inputFile,
                             char *inputFilename,
                             int *skipInputFile);

void handleErrorsException(FunctionStatus status);

#endif