#include "memoryAlloc.h"

/*!
  \brief Libera uma matriz na memória
  *
  \param matrix Ponteiro para a matriz
  \param size Tamanho da matriz
*/
void freeMatrix(real_t **matrix, uint size)
{
    if (!matrix)
        return;

    for (uint i = 0; i < size; i++)
    {
        if (matrix[i])
            free(matrix[i]);
    }
    free(matrix);
}

/*!
  \brief Libera um array na memória
  *
  \param array Ponteiro para o array
*/
void freeArray(void *array)
{
    if (!array)
        return;

    free(array);
}

/*!
  \brief Aloca uma matriz do tipo ponteiro de ponteiro na memória
  *
  \param size Tamanho da matriz
  *
  \returns O ponteiro para a matriz de double alocada
*/
real_t **allocMatrix(uint size)
{
    real_t **matrix = calloc(size, sizeof(real_t *));

    if (!matrix)
        return NULL;

    for (uint i = 0; i < size; i++)
    {
        matrix[i] = calloc(size, sizeof(real_t));

        if (!matrix[i])
        {
            for (uint j = i - 1; j >= 0; j--)
                free(matrix[j]);
            free(matrix);

            return NULL;
        }
    }

    return matrix;
}

/*!
  \brief Aloca um array de unsigned int
  *
  \param size Tamanho do array
  *
  \returns O ponteiro para o array de unsigned int alocado
*/
uint *allocUintArray(uint size)
{
    uint *array = calloc(size, sizeof(uint));

    if (!array)
        return NULL;
    return array;
}

/*!
  \brief Aloca um array de double
  *
  \param size Tamanho do array
  *
  \returns O ponteiro para o array de double alocado
*/
real_t *allocDoubleArray(uint size)
{
    real_t *array = calloc(size, sizeof(real_t));

    if (!array)
        return NULL;
    return array;
}

/*!
  \brief Libera na memória as principais estruturas utilizadas no programa
  *
  \param A Ponteiro para a matriz a ser invertida
  \param L Ponteiro para a matriz de multiplicadores L
  \param U Ponteiro para a matriz triangularizada
  \param invertedMatrix Ponteiro para a matriz inversa encontrada
  \param lineSwaps Ponteiro para o array de trocas de linha
  \param iterationsNorm Ponteiro para o array das normas das iterações do refinamento
  \param size Tamanho da matriz a ser invertida
  \param inputFile Ponteiro para o arquivo de entrada
  \param outputFile Ponteiro para o arquivo de saida
*/
void freeMainMemory(
    real_t **A,
    real_t **L,
    real_t **U,
    real_t **invertedMatrix,
    uint *lineSwaps,
    real_t *iterationsNorm,
    uint size,
    FILE *inputFile,
    FILE *outputFile)
{
    freeMatrix(A, size);
    freeMatrix(L, size);
    freeMatrix(U, size);
    freeMatrix(invertedMatrix, size);
    freeArray(lineSwaps);
    freeArray(iterationsNorm);

    if (inputFile)
        fclose(inputFile);
    if (outputFile)
        fclose(outputFile);
}

/*!
  \brief Verifica se algum dos ponteiros está nulo e retorna um status de erro ou sucesso
  *
  \param A Ponteiro para a matriz a ser invertida
  \param L Ponteiro para a matriz de multiplicadores L
  \param U Ponteiro para a matriz triangularizada
  \param invertedMatrix Ponteiro para a matriz inversa encontrada
  \param lineSwaps Ponteiro para o array de trocas de linha
  \param iterationsNorm Ponteiro para o array das normas das iterações do refinamento
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus verifyMainAllocs(real_t **A,
                                real_t **L,
                                real_t **U,
                                real_t **invertedMatrix,
                                uint *lineSwaps,
                                real_t *iterationsNorm)
{
    FunctionStatus status = success;

    if (!A || !L || !U || !invertedMatrix || !lineSwaps || !iterationsNorm)
        status = allocErr;

    return status;
}