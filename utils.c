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

void cleanMatrix(real_t **matrix, uint size)
{
  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      matrix[i][j] = 0.0;
}

void setMainDiagonal(real_t **matrix, real_t value, uint size)
{
  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      if (i == j)
        matrix[i][j] = value;
}

void initArrayWithIndexes(int *array, uint size)
{
  for (int i = 0; i < size; i++)
    array[i] = i;
}

void applyLineSwaps(uint *lineSwaps, real_t **matrix, uint size)
{
  real_t *curLines[size];
  for (int i = 0; i < size; i++)
    curLines[i] = matrix[i];

  for (int i = 0; i < size; i++)
    matrix[i] = curLines[lineSwaps[i]];
}

void applyLineSwapsOnArray(uint *lineSwaps, real_t *array, uint size)
{
  real_t curValues[size];
  for (int i = 0; i < size; i++)
    curValues[i] = array[i];

  for (int i = 0; i < size; i++)
    array[i] = curValues[lineSwaps[i]];
}

void copyArray(real_t *origin, real_t *destination, uint size)
{
  for (uint i = 0; i < size; i++)
    destination[i] = origin[i];
}

void copyMatrix(real_t **origin, real_t **destination, uint size)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
      destination[i][j] = origin[i][j];
  }
}

void printMatrix(real_t **matrix, uint size)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
      printf("%.15g ", matrix[i][j]);
    printf("\n");
  }
}

void copyColumnToArray(real_t **matrix, real_t *array, uint column, uint size)
{
  for (uint i = 0; i < size; i++)
    array[i] = matrix[i][column];
}

void readMatrix(real_t **matrix, uint size)
{
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      scanf("%lg", &matrix[i][j]);
    }
  }
}

FunctionStatus readMatrixFromFile(real_t **matrix, uint size, FILE *inputFile)
{
  FunctionStatus status = success;

  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      if(fscanf(inputFile, "%lg", &matrix[i][j]) == -1)
        status = missingData;

  return status;
}

void printMatrixInFile(real_t **matrix, uint size, FILE *outputFile)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
      fprintf(outputFile, "%.15g ", matrix[i][j]);
    fprintf(outputFile, "\n");
  }
}

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