#include <string.h>
#include <math.h>
#include "utils.h"

/*  Retorna tempo em milisegundos

    Forma de uso:

    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/
double timestamp(void)
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
  return ((double)(tp.tv_sec * 1.0e3 + tp.tv_nsec * 1.0e-6));
}

/*!
  \brief Inicia uma matriz identidade
  *
  \param matrix Ponteiro para a matriz
  \param size tamanho da matriz
*/
void initIdentityMatrix(real_t **matrix, uint size)
{
  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
    {
      if (i == j)
        matrix[i][j] = 1.0;
      else
        matrix[i][j] = 0.0;
    }
}

/*!
  \brief Preenche uma matriz com 0.0
  *
  \param matrix Ponteiro para a matriz
  \param size Tamanho da matriz
*/
void cleanMatrix(real_t **matrix, uint size)
{
  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      matrix[i][j] = 0.0;
}

/*!
  \brief Preenche a diagonal principal da matriz com um valor especificado
  *
  \param matrix Ponteiro para a matriz
  \param value Valor para a diagonal principal
  \param size Tamanho da matriz
*/
void setMainDiagonal(real_t **matrix, real_t value, uint size)
{
  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      if (i == j)
        matrix[i][j] = value;
}

/*!
  \brief Preenche um array com o valor dos seus índices
  *
  \param array Ponteiro para o array
  \param size Tamanho do array
*/
void initArrayWithIndexes(int *array, uint size)
{
  for (int i = 0; i < size; i++)
    array[i] = i;
}

/*!
  \brief Aplica na matriz as trocas de linhas que estão armazenadas
  *
  \param lineSwaps Ponteiro para o array de troca de linhas
  \param matrix Ponteiro para a matriz
  \param size Tamanho da matriz
*/
void applyLineSwaps(uint *lineSwaps, real_t **matrix, uint size)
{
  real_t *curLines[size];
  for (int i = 0; i < size; i++)
    curLines[i] = matrix[i];

  for (int i = 0; i < size; i++)
    matrix[i] = curLines[lineSwaps[i]];
}

/*!
  \brief Aplica em um array as trocas de linhas que estão armazenadas
  *
  \param lineSwaps Ponteiro para o array de troca de linhas
  \param array Ponteiro para o array
  \param size Tamanho do array
*/
void applyLineSwapsOnArray(uint *lineSwaps, real_t *array, uint size)
{
  real_t curValues[size];
  for (int i = 0; i < size; i++)
    curValues[i] = array[i];

  for (int i = 0; i < size; i++)
    array[i] = curValues[lineSwaps[i]];
}

/*!
  \brief Copia os valores de um array para outro
  *
  \param origin Ponteiro para o array de origem
  \param destination Ponteiro para o array de destino
  \param size Tamanho dos arrays
*/
void copyArray(real_t *origin, real_t *destination, uint size)
{
  for (uint i = 0; i < size; i++)
    destination[i] = origin[i];
}

/*!
  \brief Copia os valores de uma matriz para outra
  *
  \param origin Ponteiro para a matriz de origem
  \param destination Ponteiro para a matriz de destino
  \param size Tamanho das matrizes
*/
void copyMatrix(real_t **origin, real_t **destination, uint size)
{
  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      destination[i][j] = origin[i][j];
}

/*!
  \brief Imprime os valores de uma matriz
  *
  \param matrix Ponteiro para a matriz
  \param size Tamanho da matriz
*/
void printMatrix(real_t **matrix, uint size)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
      printf("%.15g ", matrix[i][j]);
    printf("\n");
  }
}

/*!
  \brief Copia a coluna de uma matriz para um array
  *
  \param matrix Ponteiro para a matriz
  \param array Ponteiro para o array
  \param column Índice da coluna a ser copiada
  \param size Tamanho da matriz
*/
void copyColumnToArray(real_t **matrix, real_t *array, uint column, uint size)
{
  for (uint i = 0; i < size; i++)
    array[i] = matrix[i][column];
}

/*!
  \brief Lê uma matriz de um arquivo
  *
  \param matrix Ponteiro para a matriz a ser armazenada
  \param size Tamanho da matriz
  \param inputFile Ponteiro arquivo a ser lido
*/
FunctionStatus readMatrixFromFile(real_t **matrix, uint size, FILE *inputFile)
{
  FunctionStatus status = success;

  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      if (fscanf(inputFile, "%lg", &matrix[i][j]) == -1)
        status = missingData;

  return status;
}

/*!
  \brief Imprime uma matriz em um arquivo
  *
  \param matrix Ponteiro para a matriz
  \param size Tamanho da matriz
  \param outputFile Ponteiro para arquivo a ser escrito
*/
void printMatrixInFile(real_t **matrix, uint size, FILE *outputFile)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
      fprintf(outputFile, "%.15g ", matrix[i][j]);
    fprintf(outputFile, "\n");
  }
}

/*!
  \brief Multiplica dois valores double e verifica se ocorreu Inf ou NaN
  *
  \param result Ponteiro para a variável de resultado
  \param number1 Valor 1 a ser multiplicado
  \param number2 Valor 2 a ser multiplicado
*/
FunctionStatus multiplyDouble(real_t *result, real_t number1, real_t number2)
{
  real_t operation = number1 * number2;

  if (isinf(operation))
    return infErr;
  else if (isnan(operation))
    return nanErr;

  *result = operation;
  return success;
}

/*!
  \brief Divide dois valores double e verifica se ocorreu Inf ou NaN
  *
  \param result Ponteiro para a variável de resultado
  \param number1 Valor do numerador
  \param number2 Valor do denominador
*/
FunctionStatus divideDouble(real_t *result, real_t number1, real_t number2)
{
  real_t operation = number1 / number2;

  if (isinf(operation))
    return infErr;
  else if (isnan(operation))
    return nanErr;

  *result = operation;
  return success;
}

/*!
  \brief Calcula o valor da determinante de uma matriz triangular
  *
  \param result Ponteiro para a variável de resultado
  \param matrix Ponteiro para a matriz
  \param size Tamanho da matriz
*/
FunctionStatus detTriangularMatrix(real_t *result, real_t **matrix, uint size)
{
  real_t det = 1;
  int status = success;

  for (uint i = 0; i < size; i++)
  {
    status = multiplyDouble(&det, det, matrix[i][i]);
    if (status != success)
      return status;
  }

  *result = det;
  return status;
}