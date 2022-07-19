#ifndef __INTERFACE__
#define __INTERFACE__

#include "utils.h"

void handleArgs(int argc, char *argv[], char *inputFilename, char *outputFilename, int *iterations);

void handleInput(char *filename, real_t **A, uint *size);

#endif