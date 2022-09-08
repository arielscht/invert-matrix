#include "../memoryAlloc/memoryAlloc.h"
#include "../sislin/sislin.h"

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

    if (matrix[0])
        free(matrix[0]);

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

    matrix[0] = calloc(size * size, sizeof(real_t));

    if (!matrix[0])
        return NULL;

    for (uint i = 1; i < size; i++)
        matrix[i] = matrix[i - 1] + size;

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

/*!
  \brief Verifica se algum dos ponteiros está nulo e retorna um status de erro ou sucesso
  *
  \param identity Ponteiro para a matriz identidade
  \param residuals Ponteiro para a matriz de residuos
  \param curSol Ponteiro para a solução atual
  \param auxSL Ponteiro para o sistema linear auxiliar
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus verifyRefinementAllocs(real_t **identity,
                                      real_t **residuals,
                                      real_t *curSol,
                                      real_t *indTerms)
{
    FunctionStatus status = success;

    if (!identity || !residuals || !curSol || !indTerms)
        status = allocErr;

    return status;
}

/*!
  \brief Faz todas as liberações de memória necessárias
  *
  \param identity Ponteiro para a matriz identidade
  \param residuals Ponteiro para a matriz de residuos
  \param curSol Ponteiro para a solução atual
  \param auxSL Ponteiro para o sistema linear auxiliar
  \param size Tamanho de matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
void freeRefinementMemory(real_t **identity,
                          real_t **residuals,
                          real_t *curSol,
                          real_t *indTerms,
                          uint size)
{
    freeMatrix(identity, size);
    freeMatrix(residuals, size);
    freeArray(curSol);
    freeArray(indTerms);
}

/*!
  \brief Verifica se algum dos ponteiros está nulo e retorna um status de erro ou sucesso
  *
  \param auxSL Ponteiro para o sistema linear auxiliar
  \param sol Ponteiro para a solução
  \param identity Ponteiro para a matriz identidade
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus verifyReverseMatrixAllocs(SistLinear_t *auxSL,
                                         real_t *sol,
                                         real_t **identity)
{
    FunctionStatus status = success;

    if (!auxSL || !sol || !identity)
        status = allocErr;

    return status;
}

/*!
  \brief Faz todas as liberações de memória necessárias
  *
  \param auxSL Ponteiro para o sistema linear auxiliar
  \param sol Ponteiro para a solução
  \param identity Ponteiro para a matriz identidade
  \param size Tamanho de matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
void freeReverseMatrixMemory(SistLinear_t *auxSL,
                             real_t *sol,
                             real_t **identity,
                             uint size)
{
    liberaSisLin(auxSL);
    freeArray(sol);
    freeMatrix(identity, size);
}