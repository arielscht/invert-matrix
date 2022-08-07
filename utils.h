#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/*!
  \brief Tipo usado para representar valores em ponto flutuante
*/
typedef double real_t;

/*!
  \brief Tipo usado para representar valores unsigned int
*/
typedef unsigned int uint;

/*!
  \brief Enum para o status das funções do programa
*/
typedef enum
{
    success = 0,
    infErr,
    nanErr,
    allocErr,
    nonInvertibleErr,
    fileErr,
    fileInputEmpty,
    missingData,
} FunctionStatus;

double timestamp(void);
void initIdentityMatrix(real_t **matrix, unsigned int size);
void initArrayWithIndexes(int *array, uint size);
void applyLineSwaps(uint *lineSwaps, real_t **matrix, uint size);
void applyLineSwapsOnArray(uint *lineSwaps, real_t *array, uint size);
void copyMatrix(real_t **origin, real_t **destination, uint size);
void printMatrix(real_t **matrix, uint size);
void copyArray(real_t *origin, real_t *destination, uint size);
void copyColumnToArray(real_t **matrix, real_t *array, uint column, uint size);
void setMainDiagonal(real_t **matrix, real_t value, uint size);
void cleanMatrix(real_t **matrix, uint size);
void printMatrixInFile(real_t **matrix, uint size, FILE *outputFile);
FunctionStatus readMatrixFromFile(real_t **matrix, uint size, FILE *outputFile);
FunctionStatus detTriangularMatrix(real_t *result, real_t **matrix, uint size);
FunctionStatus multiplyDouble(real_t *result, real_t number1, real_t number2);
FunctionStatus divideDouble(real_t *result, real_t number1, real_t number2);
FunctionStatus calcDet(real_t *result, real_t **matrix, int size);

#endif // __UTILS_H__
