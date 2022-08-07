#include <stdio.h>
#include <float.h>
#include <math.h>
#include "sisUtils.h"

/*!
  \brief Encontra o pivô de uma coluna para a eliminação de gauss
  *
  \param A Ponteiro para a matriz de coeficientes
  \param curColumn O índice da coluna atual na eliminação
  \param size Tamanho da matriz de coeficientes
  *
  \returns O índice da linha do pivô encontrado
*/
uint findPivot(real_t **A,
               uint curColumn,
               uint size)
{
    uint pivot = curColumn;

    for (uint line = curColumn + 1; line < size; line++)
        if ((A[line][curColumn] - A[pivot][curColumn]) > DBL_EPSILON)
            pivot = line;

    return pivot;
}

/*!
  \brief Troca duas linhas de uma matriz do tipo ponteiro para ponteiro
  *
  \param matrix Ponteiro para a matriz
  \param line1 Índice da linha 1 a ser trocada
  \param line2 Índice da linha 2 a ser trocada
*/
void swapLines(real_t **matrix,
               uint line1,
               uint line2)
{
    real_t *auxLine = matrix[line1];
    matrix[line1] = matrix[line2];
    matrix[line2] = auxLine;
}

/*!
  \brief Aplica a retrosubstituição em um sistema linear triangular superior
  *
  \param SL Ponteiro para o sistema linear
  \param solution Ponteiro para o array onde a solução será armazenada
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus retroSubstitution(SistLinear_t *SL,
                                 real_t *solution)
{
    uint size = SL->n;
    FunctionStatus status = success;
    real_t mult;

    for (uint line = size; line >= 1; line--)
    {
        // variable used due to uint never being less than 0
        uint actualLine = line - 1;
        solution[actualLine] = SL->b[actualLine];
        for (uint column = size - 1; column > actualLine; column--)
        {
            if ((status = multiplyDouble(&mult, SL->A[actualLine][column], solution[column])) != success)
                return status;
            solution[actualLine] -= mult;
        }
        if ((status = divideDouble(&solution[actualLine], solution[actualLine], SL->A[actualLine][actualLine])) != success)
            return status;
    }
    return status;
}

/*!
  \brief Aplica a retrosubstituição em um sistema linear triangular inferior
  *
  \param SL Ponteiro para o sistema linear
  \param solution Ponteiro para o array onde a solução será armazenada
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus reverseRetroSubstitution(SistLinear_t *SL,
                                        real_t *solution)
{
    uint size = SL->n;
    FunctionStatus status = success;
    real_t mult;

    for (int line = 0; line < size; line++)
    {
        solution[line] = SL->b[line];
        for (int column = 0; column < line; column++)
        {
            if ((status = multiplyDouble(&mult, SL->A[line][column], solution[column])) != success)
                return status;
            solution[line] -= mult;
        }
        if ((status = divideDouble(&solution[line], solution[line], SL->A[line][line])) != success)
            return status;
    }

    return status;
}

/*!
  \brief Calcula a norma L2 dos resíduos
  *
  \param residual Ponteiro para a matriz de residuos
  \param size Tamanho da matriz de residuos
  \param result Ponteiro para a varíavel de resultado
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcL2Norm(real_t **residual,
                          uint size,
                          real_t *result)
{
    real_t sum = 0.0;
    FunctionStatus status = success;
    real_t mult;

    for (uint i = 0; i < size; i++)
        for (uint j = 0; j < size; j++)
        {
            if ((status = multiplyDouble(&mult, residual[i][j], residual[i][j])) != success)
                return status;
            sum += mult;
        }
    *result = sqrt(sum);
    return status;
}

/*!
  \brief Calcula o resíduo de um sistema linear
  *
  \param SL Ponteiro para o sistema linear
  \param solution Ponteiro para o array de soluções do sistema
  \param residual Ponteiro para o array onde os resíduos serão armazenados
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcResidual(SistLinear_t *SL,
                            real_t *solution,
                            real_t *residual)
{
    uint size = SL->n;
    FunctionStatus status = success;
    real_t mult;

    for (size_t i = 0; i < size; i++)
    {
        residual[i] = SL->b[i];
        for (size_t j = 0; j < size; j++)
        {
            if ((status = multiplyDouble(&mult, solution[j], SL->A[i][j])) != success)
                return status;
            residual[i] -= mult;
        }
    }

    return status;
}

/*!
  \brief Função que vai calcular o resíduo da iteração atual do refinamento
  *
  \param identity Ponteiro para a matriz identidade
  \param auxSL Ponteiro para o sistema auxiliar durante as contas
  \param solution Ponteiro para a matriz inversa
  \param curSol Ponteiro para o vetor de solução atual do refinamento
  \param residuals Ponteiro para a matriz de residuos
  \param size Tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcRefinementResidual(real_t **identity,
                                      SistLinear_t *auxSL,
                                      real_t **solution,
                                      real_t *curSol,
                                      real_t **residuals,
                                      uint size)
{
    FunctionStatus status = success;

    for (int i = 0; i < size && status == success; i++)
    {
        copyColumnToArray(identity, auxSL->b, i, size);
        copyColumnToArray(solution, curSol, i, size);

        status = calcResidual(auxSL, curSol, residuals[i]);
    }
    return status;
}

/*!
  \brief Função que vai calcular a nova aproximação do refinamento para a próxima iteração
  *
  \param lineSwaps Ponteiro para o vetor de troca de linhas
  \param residuals Ponteiro para a matriz de residuos
  \param L Ponteiro para a matriz L
  \param auxSL Ponteiro para o sistema auxiliar durante as contas
  \param curSol Ponteiro para o vetor de solução atual do refinamento
  \param solution Ponteiro para o vetor de solução da interação anterior
  \param U Ponteiro para a matriz U
  \param size Tamanho da matriz
  *
  \returns O status de execução da função do tipo FunctionStatus
*/
FunctionStatus calcRefinementNewApproximation(uint *lineSwaps,
                                              real_t **residuals,
                                              real_t **L,
                                              SistLinear_t *auxSL,
                                              real_t *curSol,
                                              real_t **solution,
                                              real_t **U,
                                              uint size)
{
    FunctionStatus status = success;

    for (int i = 0; i < size && status == success; i++)
    {
        applyLineSwapsOnArray(lineSwaps, residuals[i], size);
        copyArray(residuals[i], auxSL->b, size);
        copyMatrix(L, auxSL->A, size);

        if ((status = reverseRetroSubstitution(auxSL, curSol)) != success)
            continue;

        copyMatrix(U, auxSL->A, size);
        copyArray(curSol, auxSL->b, size);

        if ((status = retroSubstitution(auxSL, curSol)) != success)
            continue;
        // soma a solução do resíduo com a solução anterior para obter a nova apromixação
        for (int j = 0; j < size; j++)
            solution[j][i] += curSol[j];
    }

    return status;
}
