#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Valor absoluto de um número. Alternativa ao uso da função 'fabs()'
#define ABS(num) ((num) < 0.0 ? -(num) : (num))

// real_t: tipo usado para representar valores em ponto flutuante
typedef double real_t;

// uing: tipo usado para representar valores unsigned int
typedef unsigned int uint;

// string_t: tipo usado para representar ponteiros para char/strings
typedef char *string_t;

// rtime_t: tipo usado para representar valores de tempo em ponto flutuante
typedef double rtime_t;

// SIMD alignment macros
#define ALIGN_64 __attribute__((aligned(64)))
#define ALIGN_32 __attribute__((aligned(32)))
#define ALIGN_16 __attribute__((aligned(16)))

// Número máximo de dígitos em um número
#define numDigits(n) 6 // ( (int) log10(n) + 1 )

// Macro para verificar de valor 'n' é potência de 2 ou não
#define isPot2(n) (n && !(n & (n - 1)))

// Funções
double timestamp(void);
string_t markerName(string_t baseName, int n);

// ERROR_ENUM
typedef enum
{
    success = 0,
    infErr,
    nanErr,
    allocErr,
    nonInvertibleErr,
    fileInputErr,
    fileOutputErr, 
    fileInputEmpty,
} FunctionStatus;

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
void readMatrix(real_t **matrix, uint size);
void readMatrixFromFile(real_t **matrix, uint size, FILE *outputFile);
void printMatrixInFile(real_t **matrix, uint size, FILE *outputFile);
FunctionStatus detTriangularMatrix(real_t *result, real_t **matrix, uint size);
FunctionStatus multiplyDouble(real_t *result, real_t number1, real_t number2);
FunctionStatus divideDouble(real_t *result, real_t number1, real_t number2);

#endif // __UTILS_H__
