#ifndef __INTERFACE__
#define __INTERFACE__

#include "utils.h"

void handleArgs(int argc,
                char *argv[],
                char *inputFilename,
                char *outputFilename,
                int *iterations,
                uint *size);

void handleInput(
    FILE **inputFile,
    char *filename);

void handleOutput(
    FILE **outputFile,
    char *filename);

#endif