#include <stdio.h>
#include "methods.h"
#include "utils.h"
#include "math.h"

uint findPivot(real_t **A, uint curColumn, uint size)
{
    uint pivot = curColumn;

    for (uint line = curColumn + 1; line < size; line++)
        if ((A[line][curColumn] - A[pivot][curColumn]) > ERRO_COMPARISON)
            pivot = line;

    return pivot;
}

void swapLines(real_t **matrix, uint line1, uint line2)
{
    real_t *auxLine = matrix[line1];
    matrix[line1] = matrix[line2];
    matrix[line2] = auxLine;
}

void retroSubstitution(SistLinear_t *SL, real_t *solution)
{
    uint size = SL->n;
    for (uint line = size; line >= 1; line--)
    {
        // variable used due to uint never being less than 0
        uint actualLine = line - 1;
        solution[actualLine] = SL->b[actualLine];
        for (uint column = size - 1; column > actualLine; column--)
        {
            solution[actualLine] -= SL->A[actualLine][column] * solution[column];
        }
        solution[actualLine] /= SL->A[actualLine][actualLine];
    }
}

void reverseRetroSubstitution(SistLinear_t *SL, real_t *solution)
{
    uint size = SL->n;

    for (int line = 0; line < size; line++)
    {
        solution[line] = SL->b[line];
        for (int column = 0; column < line; column++)
            solution[line] -= SL->A[line][column] * solution[column];
        solution[line] /= SL->A[line][line];
    }
}

real_t calcL2Norm(real_t **residual, uint size)
{
    real_t sum = 0.0;
    for (uint i = 0; i < size; i++)
        for (uint j = 0; j < size; j++)
            sum += residual[i][j] * residual[i][j];
    return sqrt(sum);
}

void calcResidual(SistLinear_t *SL, real_t *solution, real_t *residual)
{
    uint size = SL->n;
    for (size_t i = 0; i < size; i++)
    {
        residual[i] = SL->b[i];
        for (size_t j = 0; j < size; j++)
            residual[i] -= solution[j] * SL->A[i][j];
    }
}

void refinement(real_t **A, real_t **solution, uint size, real_t erro)
{
    // loop calculando o residuo para cada coluna da inversa
    // armazena o resíduo em uma matriz
    //  calcula a normal2 que é a soma de cada elemento da matriz ao quadrado
    //  testa condicao de parada
    //  resolve para o resíduo atual
    //  soma a solução do resíduo com a solução do sistema anterior
    // repete
    real_t **residuals = allocMatrix(size);
    real_t *curSol = allocArray(size);
    real_t norm = 0.0;
    int counter = 0;
    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    copyMatrix(A, auxSL->A, size);
    real_t **identity = allocMatrix(size);
    initIdentityMatrix(identity, size);

    do
    {
        for (int i = 0; i < size; i++)
        {
            copyColumnToArray(identity, auxSL->b, i, size);
            copyColumnToArray(solution, curSol, i, size);
            calcResidual(auxSL, curSol, residuals[i]);
        }

        norm = calcL2Norm(residuals, size);

        for (int i = 0; i < size; i++)
        {
        }
    } while (counter < MAXIT && norm > erro);

    freeMatrix(identity, size);
    liberaSisLin(auxSL);
}

int gaussElimination(real_t **A,
                     real_t **L,
                     real_t **identityMatrix,
                     uint size)
{
    for (uint line = 0; line < size - 1; line++)
    {
        uint pivotLine = findPivot(A, line, size);
        if (pivotLine != line)
        {
            swapLines(A, line, pivotLine);
            if (identityMatrix != NULL)
                swapLines(identityMatrix, line, pivotLine);
            if (L != NULL)
                swapLines(L, line, pivotLine);
        }

        for (uint auxLine = line + 1; auxLine < size; auxLine++)
        {
            real_t m = A[auxLine][line] / A[line][line];
            A[auxLine][line] = 0.0;

            if (L != NULL)
                L[auxLine][line] = m;

            for (uint column = line + 1; column < size; column++)
                A[auxLine][column] -= A[line][column] * m;
        }
    }

    return 0;
}

int factorizationLU(real_t **A,
                    real_t **L,
                    real_t **U,
                    real_t **identity,
                    uint size)
{
    copyMatrix(A, U, size);

    initIdentityMatrix(identity, size);
    cleanMatrix(L, size);

    gaussElimination(U, L, identity, size);
    setMainDiagonal(L, 1.0, size);

    return 0;
}

int reverseMatrix(real_t **A,
                  real_t **L,
                  real_t **U,
                  real_t **identity,
                  real_t **invertedMatrix,
                  uint size,
                  real_t *tTotal)
{
    factorizationLU(A, L, U, identity, size);

    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    real_t *sol = calloc(size, sizeof(real_t));

    for (uint i = 0; i < size; i++)
    {
        copyColumnToArray(identity, auxSL->b, i, size);
        copyMatrix(L, auxSL->A, size);
        reverseRetroSubstitution(auxSL, sol);
        copyMatrix(U, auxSL->A, size);
        copyArray(sol, auxSL->b, size);
        retroSubstitution(auxSL, sol);
        for (uint j = 0; j < size; j++)
            invertedMatrix[j][i] = sol[j];
    }

    liberaSisLin(auxSL);
    free(sol);
}
