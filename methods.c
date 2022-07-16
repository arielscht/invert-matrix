#include "methods.h"
#include "utils.h"

uint findPivot(SistLinear_t *SL, uint curColumn)
{
    uint size = SL->n;
    uint pivot = curColumn;

    for (uint line = curColumn + 1; line < size; line++)
        if ((SL->A[line][curColumn] - SL->A[pivot][curColumn]) > ERRO_COMPARISON)
            pivot = line;

    return pivot;
}

void swapLines(SistLinear_t *SL, uint line1, uint line2)
{
    real_t *auxLine = SL->A[line1];
    real_t auxTerm = SL->b[line1];
    SL->A[line1] = SL->A[line2];
    SL->A[line2] = auxLine;
    SL->b[line1] = SL->b[line2];
    SL->b[line2] = auxTerm;
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

/*!
    \brief          Gauss elimination method

    \param SL       Linear system pointer
    \param solution Solution(array) pointer
    \param tTotal   Total time during the execution of this method

    \return         Error code. 0 to success
*/
int gaussElimination(SistLinear_t *SL, real_t *solution, real_t *tTotal)
{
    uint size = SL->n;
    *tTotal = timestamp();

    for (uint line = 0; line < size - 1; line++)
    {
        uint pivot = findPivot(SL, line);
        if (pivot != line)
            swapLines(SL, line, pivot);

        for (uint auxLine = line + 1; auxLine < size; auxLine++)
        {
            real_t m = SL->A[auxLine][line] / SL->A[line][line];
            SL->A[auxLine][line] = 0.0;

            for (uint column = line + 1; column < size; column++)
                SL->A[auxLine][column] -= SL->A[line][column] * m;
            SL->b[auxLine] -= SL->b[line] * m;
        }
    }

    retroSubstitution(SL, solution);

    *tTotal = timestamp() - *tTotal;
    return 0;
}