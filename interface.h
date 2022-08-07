#ifndef __INTERFACE__
#define __INTERFACE__

#include "utils.h"

void handleArgs(int argc,
                char *argv[],
                char *inputFilename,
                char *outputFilename,
                int *iterations,
                uint *size);

FunctionStatus handleFile(FILE **inputFile,
                          char *filename,
                          char *fileMode);

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

FunctionStatus handleMainInput(uint *size,
                               FILE **inputFile,
                               char *inputFilename,
                               int *skipInputFile);

void handleErrorsException(FunctionStatus status);

#endif