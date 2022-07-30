#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void handleArgs(int argc,
                char *argv[],
                char *inputFilename,
                char *outputFilename,
                int *iterations)
{
    int option;
    while ((option = getopt(argc, argv, "e:s:r:i:h::")) != -1)
    {
        switch (option)
        {
        case 'e':
            strcpy(inputFilename, optarg);
            break;
        case 's':
            strcpy(outputFilename, optarg);
            break;
        case 'r':
            /* code */
            break;
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
}

int handleInput(char *filename, real_t ***A, uint *size)
{
    FILE *inputFile = stdin;
    if (filename)
    {
        inputFile = fopen(filename, "r");
        if (!inputFile)
        {
            fprintf(stderr, "Error reading the input file.\n");
            return 1;
        }
    }

    fscanf(inputFile, "%u", size);
    printf("SIZE: %u\n", *size);
    *A = allocMatrix(*size);

    for (uint i = 0; i < *size; i++)
        for (uint j = 0; j < *size; j++)
            fscanf(inputFile, "%lg", &((*A)[i][j]));

    fclose(inputFile);
}