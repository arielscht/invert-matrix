#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "../sislin/sislin.h"
#include "../utils/utils.h"

/*!
  \brief Lida com os argumentos do programa
  *
  \param argc Parâmetro argc da main, número de argumentos passados
  \param argv Array de argumentos da execução do programa
  \param inputFilename Ponteiro para o nome do arquivo de entrada
  \param outputFilename Ponteiro para o nome do arquivo de saída
  \param iterations Ponteiro para a variável que armazena o número de iterações a serem executadas
  \param size Ponteiro para a variável que armazena o tamanho da matriz
*/
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

/*!
  \brief Abre um arquivo e retorna o ponteiro por parâmetro
  *
  \param inputFile Ponteiro para o ponteiro do arquivo
  \param filename Caminho do arquivo a ser aberto
  \param fileMode Modo de abertura do arquivo ex: "r" para leitura, "w" para escrita
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus handleFile(FILE **inputFile, char *filename, char *fileMode)
{
    FunctionStatus status = success;

    if (filename[0] != '\0')
    {
        *inputFile = fopen(filename, fileMode);
        if (!*inputFile)
            status = fileErr;
    }

    return status;
}

/*!
  \brief Imprime os resultadoes finais no arquivo de saída
  *
  \param outputFile Ponteiro para o arquivo de saída
  \param iterationsNorm Ponteiro para o vetor das normas do refinamento
  \param totalTimeFactorization Tempo total da fatoração LU
  \param averageTimeRefinement Tempo médio das iterações do refinamento
  \param averageTimeNorm Tempo médio do cálculo da norma
  \param size Tamanho das matrizes
  \param invertedMatrix Ponteiro para a matriz inversa obtida
  \param iterations O número de iterações do refinamento executadas
*/
void printFinalOutput(FILE *outputFile,
                      real_t *iterationsNorm,
                      real_t totalTimeFactorization,
                      real_t totalTimeFirstSolution,
                      real_t averageTimeRefinement,
                      real_t averageTimeNorm,
                      real_t averageTimeResidual,
                      uint size,
                      real_t **invertedMatrix,
                      uint iterations)
{
    fprintf(outputFile, "#\n");
    for (uint i = 0; i < iterations; i++)
        fprintf(outputFile, "# iter %d: <||%.15g||>\n", i + 1, iterationsNorm[i]);

    fprintf(outputFile, "# Tempo LU: %.15g\n", totalTimeFactorization);
    fprintf(outputFile, "# Tempo iter: %.15g\n", averageTimeRefinement);
    fprintf(outputFile, "# Tempo norma: %.15g\n", averageTimeNorm);
    fprintf(outputFile, "# Tempo residuo: %.15g\n", averageTimeResidual);
    fprintf(outputFile, "# Tempo primeira solucao: %.15g\n", totalTimeFirstSolution);
    fprintf(outputFile, "#\n");
    fprintf(outputFile, "%d\n", size);
    printMatrixInFile(invertedMatrix, size, outputFile);
}

/*!
  \brief Imprime em stderr a mensagem correspondente para cada código de erro
  *
  \param status Código de erro do tipo FunctionStatus
*/
void handleErrorsException(FunctionStatus status)
{
    switch (status)
    {
    case infErr:
        fprintf(stderr, "Some operation went to the infinity.");
        break;
    case nanErr:
        fprintf(stderr, "Some arithmetic operation returned a NaN.");
        break;
    case allocErr:
        fprintf(stderr, "Some allocation memory went wrong.");
        break;
    case nonInvertibleErr:
        fprintf(stderr, "The given matrix is not invertible.");
        break;
    case fileErr:
        fprintf(stderr, "Failed to open file.");
        break;
    case fileInputEmpty:
        fprintf(stderr, "The given input is empty.");
        break;
    case missingData:
        fprintf(stderr, "Missing data in the input file.");
        break;
    default:
        fprintf(stderr, "Unknown error.");
        break;
    }
    printf("\n");
}

/*!
  \brief Inicializa a matriz a ser invertida, seja lendo um arquivo ou gerando aleatoriamente
  *
  \param skipInputFile Flag para verificar se pula a leitura do arquivo de entrada ou não
  \param A Ponteiro para a matriz A
  \param size Tamanho da matriz
  \param inputFile Ponteiro para o arquivo de entrada
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus initializeMainMatrix(int skipInputFile,
                                    real_t **A,
                                    uint size,
                                    FILE *inputFile)
{
    FunctionStatus status = success;

    if (skipInputFile)
        status = readMatrixFromFile(A, size, inputFile);
    else
        initRandomMatrix(A, generico, COEF_MAX, size);

    return status;
}

/*!
  \brief Função que vai decidir se vai ler o arquivo de entrada ou se vai gerar a matrix aleatória
  *
  \param size Tamanho da matriz
  \param inputFile Ponteiro para o arquivo de entrada
  \param inputFilename Ponteiro para o nome do arquivo
  \param skipInputFile Flag para verificar se pula a leitura do arquivo de entrada ou não
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus readMainInput(uint *size,
                             FILE **inputFile,
                             char *inputFilename,
                             int *skipInputFile)
{
    FunctionStatus status = success;

    if (!(*size) && (status = handleFile(inputFile, inputFilename, "r")) == success)
    {
        if (fscanf(*inputFile, "%d", size) == -1)
            status = fileInputEmpty;
        *skipInputFile = 1;
    }

    return status;
}