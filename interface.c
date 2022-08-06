#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void handleArgs(int argc,
                char *argv[],
                char *inputFilename,
                char *outputFilename,
                int *iterations,
                uint *size)
{
    int option;
    while ((option = getopt(argc, argv, "e:s:r:i:h::")) != -1)
    {
        switch (option)
        {
        case 'e':
            if (optarg)
                strcpy(inputFilename, optarg);
            break;
        case 's':
            if (optarg)
                strcpy(outputFilename, optarg);
            break;
        case 'r':
        {
            int arg = atoi(optarg);
            if (!arg)
            {
                fprintf(stderr, "Error: the argument of -r must be a number.\n");
                exit(1);
            }
            else if (arg <= 0)
            {
                fprintf(stderr, "Error: The dimension of the matrix must be greater than zero.\n");
                exit(1);
            }
            else
                *size = arg;
            break;
        }
        case 'i':
        {
            int arg = atoi(optarg);
            if (!arg)
            {
                fprintf(stderr, "Error: the argument of -i must be a number.\n");
                exit(1);
            }
            else if (arg <= 0)
            {
                fprintf(stderr, "Error: The number of iterations must be greater than zero.\n");
                exit(1);
            }
            else
                *iterations = arg;
            break;
        }
        case 'h':
            printf("======== Welcome to the matrix inversion program ========\n\n");
            printf("To execute the program, use the following command\n");
            printf("./invmat -e <input file> -s <output file> -r <matrix size> -i <iteration quantity>\n");
            printf("There are some optionals params, like: -e, -s, -r. The only required param is -i\n");
            exit(1);
            break;
        case '?':
            if (optopt == 'i')
                fprintf(stderr, "Option -i requires an argument.\n");
            else
                fprintf(stderr, "Unknown option. \n Execute ./invmat -h for help.\n");
            exit(1);
            break;
        default:
            abort();
            break;
        }
    }

    if (!*iterations)
    {
        fprintf(stderr, "Option -i is required.\n");
        exit(1);
    }
}

FunctionStatus handleInput(FILE **inputFile, char *filename)
{
    *inputFile = stdin;
    if (filename[0] != '\0')
    {
        *inputFile = fopen(filename, "r");
        if (!inputFile)
        {
            fprintf(stderr, "Error reading the input file.\n");
            return fileInputErr;
        }
    }
}

FunctionStatus handleOutput(FILE **outputFile, char *filename)
{
    *outputFile = stdout;
    if (filename[0] != '\0')
    {
        *outputFile = fopen(filename, "w");
        if (!outputFile)
        {
            fprintf(stderr, "Error reading the output file.\n");
            return fileOutputErr;
        }
    }
}

void printFinalOutput(FILE *outputFile,
                      real_t *iterationsNorm,
                      real_t totalTimeFactorization,
                      real_t averageTimeRefinement,
                      real_t averageTimeNorm,
                      uint size,
                      real_t **invertedMatrix,
                      uint iterations)
{
    for (uint i = 0; i < iterations; i++)
        fprintf(outputFile, "# iter %d: <||%.15g||>\n", i + 1, iterationsNorm[i]);

    fprintf(outputFile, "# Tempo LU: %10g\n", totalTimeFactorization);
    fprintf(outputFile, "# Tempo iter: %10g\n", averageTimeRefinement);
    fprintf(outputFile, "# Tempo norma: %10g\n", 0.0);
    fprintf(outputFile, "N: %d\n", size);
    printMatrixInFile(invertedMatrix, size, outputFile);
}
