#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../utils/utils.h"
#include "../sislin/sislin.h"

// Alocaçao de matriz em memória.
SistLinear_t *alocaSisLin(unsigned int n, tipoAloc_t tipo)
{
  SistLinear_t *SL = (SistLinear_t *)malloc(sizeof(SistLinear_t));

  if (SL)
  {

    SL->n = n;
    SL->tipoAloc_A = tipo;
    SL->A = (real_t **)malloc(n * sizeof(real_t *));
    SL->b = (real_t *)malloc(n * sizeof(real_t));

    if (!(SL->A) || !(SL->b))
    {
      liberaSisLin(SL);
      return NULL;
    }

    // Matriz como vetor de N ponteiros para um único vetor com N*N elementos
    if (tipo == pontVet)
    {
      SL->A[0] = (real_t *)malloc(n * n * sizeof(real_t));
      if (!(SL->A[0]))
      {
        liberaSisLin(SL);
        return NULL;
      }

      for (int i = 1; i < n; ++i)
      {
        SL->A[i] = SL->A[i - 1] + n;
      }
    }
    else if (tipo == pontPont)
    { // Matriz  como  vetor de  N  ponteiros
      // para N vetores de N elementos cada

      for (int i = 0; i < n; ++i)
        SL->A[i] = (real_t *)malloc(n * sizeof(real_t));
    }
  }

  return (SL);
}

// Liberacao de memória
void liberaSisLin(SistLinear_t *SL)
{
  if (SL)
  {
    if (SL->A)
    {
      if (SL->tipoAloc_A == pontVet)
      {
        if (SL->A[0])
          free(SL->A[0]);
      }
      else if (SL->tipoAloc_A == pontPont)
      {
        for (int i = 0; i < SL->n; ++i)
          free(SL->A[i]);
      }

      free(SL->A);
    }
    if (SL->b)
      free(SL->b);
    free(SL);
  }
}

/*!
  \brief Cria coeficientes e termos independentes do SL
  *
  \param A Ponteiro para a matriz
  \param tipo Tipo da matriz de coeficientes a ser gerada. Pode ser:
     comSolucao, eqNula, eqProporcional, eqCombLinear, hilbert
  \param coef_max Maior valor para coeficientes e termos independentes
  \param size Tamanho da matriz a ser inicializada
*/
void initRandomMatrix(real_t **A, tipoSistLinear_t tipo, real_t coef_max, uint size)
{
  unsigned int n = size;
  // para gerar valores no intervalo [0,coef_max]
  real_t invRandMax = ((real_t)coef_max / (real_t)RAND_MAX);

  if (tipo == hilbert)
  {
    for (unsigned int i = 0; i < n; ++i)
    {
      for (unsigned int j = 0; j < n; ++j)
      {
        A[i][j] = 1.0 / (real_t)(i + j + 1);
      }
    }
  }
  else
  { // inicializa sistema normal e depois altera
    // inicializa a matriz A
    for (unsigned int i = 0; i < n; ++i)
    {
      for (unsigned int j = 0; j < n; ++j)
      {
        A[i][j] = (real_t)rand() * invRandMax;
      }
    }
    if (tipo == eqNula)
    {
      // sorteia eq a ser "nula"
      unsigned int nula = rand() % n;
      for (unsigned int j = 0; j < n; ++j)
      {
        A[nula][j] = 0.0;
      }
    }
    else if (tipo == eqProporcional)
    {
      // sorteia eq a ser "proporcional" e valor
      unsigned int propDst = rand() % n;
      unsigned int propSrc = (propDst + 1) % n;
      real_t mult = (real_t)rand() * invRandMax;
      for (unsigned int j = 0; j < n; ++j)
      {
        A[propDst][j] = A[propSrc][j] * mult;
      }
    }
    else if (tipo == eqCombLinear)
    {
      // sorteia eq a ser "combLinear"
      unsigned int combDst = rand() % n;
      unsigned int combSrc1 = (combDst + 1) % n;
      unsigned int combSrc2 = (combDst + 2) % n;
      for (unsigned int j = 0; j < n; ++j)
      {
        A[combDst][j] = A[combSrc1][j] + A[combSrc2][j];
      }
    }
    else if (tipo == diagDominante)
    {
      // aumenta o valor dos termos da diagonal principal
      for (unsigned int i = 0; i < n; ++i)
      {
        real_t soma = 0.0;
        for (unsigned int j = 0; j < i; ++j)
          soma += A[i][j];
        for (unsigned int j = i + 1; j < n; ++j)
          soma += A[i][j];
        A[i][i] += soma;
      }
    }
  }
}