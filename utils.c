#include <stdio.h>
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

/* Gera string '<baseName>_n'
 * Por exemplo, se baseName = "ABC" e n = 10,
 *  Função retorna a string "ABC_10"
 * Útil para gerar marcadores para LIKWID
 */
string_t markerName(string_t baseName, int n)
{
  string_t mark = (string_t)malloc((strlen(baseName) + 1) + numDigits(n) + 1);

  sprintf(mark, "%s_%u", baseName, n);

  // printf("*** %s\n", mark);

  return mark;
}

/*
// Retorna TRUE se 'n' é uma potência de 2
// [OBSOLETO]
// Definida macro em 'utils.h' para substituir esta função
//
int isPot2(int n)
{
  int k;
  return (k = log2(n)) == log2(n) ;
}
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

void freeMatrix(real_t **matrix, uint size)
{
  if (matrix)
  {
    for (uint i = 0; i < size; i++)
    {
      if (matrix[i])
        free(matrix[i]);
    }
    free(matrix);
  }
}

real_t **allocMatrix(uint size)
{
  real_t **matrix = calloc(size, sizeof(real_t *));
  if (matrix)
  {
    for (uint i = 0; i < size; i++)
      matrix[i] = calloc(size, sizeof(real_t));
  }
  return matrix;
}

uint *allocUintArray(uint size)
{
  return calloc(size, sizeof(uint));
}

void printMatrix(real_t **matrix, uint size)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
      printf("%10g ", matrix[i][j]);
    printf("\n");
  }
}

void copyColumnToArray(real_t **matrix, real_t *array, uint column, uint size)
{
  for (uint i = 0; i < size; i++)
    array[i] = matrix[i][column];
}

void multiplyMatrix(real_t **result, real_t **matrix1, real_t **matrix2, uint size)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
    {
      real_t sum = 0.0;
      for (uint k = 0; k < size; k++)
      {
        sum += matrix1[i][k] * matrix2[k][j];
      }
      result[i][j] = sum;
    }
  }
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

void readMatrixFromFile(real_t **matrix, uint size, FILE *inputFile)
{
  for (uint i = 0; i < size; i++)
    for (uint j = 0; j < size; j++)
      fscanf(inputFile, "%lg", &matrix[i][j]);
}

void printMatrixInFile(real_t **matrix, uint size, FILE *outputFile)
{
  for (uint i = 0; i < size; i++)
  {
    for (uint j = 0; j < size; j++)
      fprintf(outputFile, "%10g ", matrix[i][j]);
    fprintf(outputFile, "\n");
  }
}