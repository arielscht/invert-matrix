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

void refinement(real_t **A,
                real_t **L,
                real_t **U,
                real_t **solution,
                uint *lineSwaps,
                uint size,
                int iterations,
                real_t erro,
                FILE *outputFile,
                real_t *tTotal)
{
    *tTotal = timestamp();
    real_t **residuals = allocMatrix(size);
    real_t *curSol = calloc(size, sizeof(real_t));
    real_t norm = 0.0;
    int counter = 1;

    SistLinear_t *auxSL = alocaSisLin(size, pontPont);
    copyMatrix(A, auxSL->A, size);

    real_t **identity = allocMatrix(size);
    initIdentityMatrix(identity, size);

    while (counter <= iterations)
    {
        // Calcula nova aproximação
        for (int i = 0; i < size; i++)
        {
            applyLineSwapsOnArray(lineSwaps, residuals[i], size);
            copyArray(residuals[i], auxSL->b, size);
            copyMatrix(L, auxSL->A, size);
            reverseRetroSubstitution(auxSL, curSol);
            copyMatrix(U, auxSL->A, size);
            copyArray(curSol, auxSL->b, size);
            retroSubstitution(auxSL, curSol);
            // soma a solução do resíduo com a solução anterior para obter a nova apromixação
            for (int j = 0; j < size; j++)
                solution[j][i] += curSol[j];
        }

        // Calcula resíduo e norma
        copyMatrix(A, auxSL->A, size);
        for (int i = 0; i < size; i++)
        {
            copyColumnToArray(identity, auxSL->b, i, size);
            copyColumnToArray(solution, curSol, i, size);
            calcResidual(auxSL, curSol, residuals[i]);
        }

        norm = calcL2Norm(residuals, size);
        fprintf(outputFile, "# iter %d: <||%.15g||>\n", counter, norm); //
        counter++;
    };

    *tTotal = timestamp() - *tTotal;
    *tTotal /= counter;
    freeMatrix(identity, size);
    freeMatrix(residuals, size);
    free(curSol);
    liberaSisLin(auxSL);
}

int gaussElimination(real_t **A,
                     real_t **L,
                     uint *lineSwaps,
                     uint size,
                     real_t *tTotal)
{
    *tTotal = timestamp();

    for (uint line = 0; line < size - 1; line++)
    {
        uint pivotLine = findPivot(A, line, size);
        if (pivotLine != line)
        {
            swapLines(A, line, pivotLine);

            real_t aux = lineSwaps[line];
            lineSwaps[line] = lineSwaps[pivotLine];
            lineSwaps[pivotLine] = aux;
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

    *tTotal = timestamp() - *tTotal;
    return 0;
}

int factorizationLU(real_t **A,
                    real_t **L,
                    real_t **U,
                    uint *lineSwaps,
                    uint size,
                    real_t *tTotal)
{
    copyMatrix(A, U, size);
    cleanMatrix(L, size);

    gaussElimination(U, L, lineSwaps, size, tTotal);
    setMainDiagonal(L, 1.0, size);

    return 0;
}

int reverseMatrix(real_t **A,
                  real_t **L,
                  real_t **U,
                  uint *lineSwaps,
                  real_t **invertedMatrix,
                  uint size,
                  real_t *tTotal)
{
    initArrayWithIndexes(lineSwaps, size);
    real_t **identity = allocMatrix(size);
    initIdentityMatrix(identity, size);

    factorizationLU(A, L, U, lineSwaps, size, tTotal);
    applyLineSwaps(lineSwaps, identity, size);

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
    freeMatrix(identity, size);
}
