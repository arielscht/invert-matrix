#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "sislin.h"

// Alocaçao de matriz em memória. 
SistLinear_t* alocaSisLin (unsigned int n, tipoAloc_t tipo)
{
  SistLinear_t *SL = (SistLinear_t *) malloc(sizeof(SistLinear_t));
  
  if ( SL ) {
    
    SL->n = n;
    SL->tipoAloc_A = tipo;
    SL->A = (real_t **) malloc(n * sizeof(real_t *));
    SL->b = (real_t *) malloc(n * sizeof(real_t));

    if (!(SL->A) || !(SL->b)) {
      liberaSisLin(SL);
      return NULL;
    }

    // Matriz como vetor de N ponteiros para um único vetor com N*N elementos
    if (tipo == pontVet) {
      SL->A[0] = (real_t *) malloc(n * n * sizeof(real_t));
      if (!(SL->A[0])) {
	liberaSisLin(SL);
	return NULL;
      }
	
      for (int i=1; i < n; ++i) {
	SL->A[i] = SL->A[i-1]+n;
      }
    }
    else if (tipo == pontPont) { // Matriz  como  vetor de  N  ponteiros
				 // para N vetores de N elementos cada
    
      for (int i=0; i < n; ++i)
	SL->A[i] = (real_t *) malloc(n * sizeof(real_t));
    }
  }
  
  return (SL);
}

// Liberacao de memória
void liberaSisLin (SistLinear_t *SL)
{
  if (SL) {
    if (SL->A) {
      if (SL->tipoAloc_A == pontVet) {
	if (SL->A[0]) free (SL->A[0]);
      }
      else if (SL->tipoAloc_A == pontPont) {
	for (int i=0; i < SL->n; ++i)
	  free (SL->A[i]);
      }
      
      free(SL->A);
    }
    if (SL->b) free(SL->b);
    free(SL);
  }
}


/*!
  \brief Cria coeficientes e termos independentes do SL
  *
  \param SL Ponteiro para o sistema linear
  \param tipo Tipo de sistema linear a ser criado. Pode ser: 
     comSolucao, eqNula, eqProporcional, eqCombLinear, hilbert 
  \param coef_max Maior valor para coeficientes e termos independentes
*/
void iniSisLin (SistLinear_t *SL, tipoSistLinear_t tipo, real_t coef_max)
{
  unsigned int n = SL->n;
  // para gerar valores no intervalo [0,coef_max]
  real_t invRandMax = ((real_t)coef_max / (real_t)RAND_MAX);

  // inicializa vetor b
  for (unsigned int i=0; i<n; ++i) {
    SL->b[i] = (real_t)rand() * invRandMax;
  }
    
  if (tipo == hilbert) {
    for (unsigned int i=0; i<n; ++i) {
      for (unsigned int j=0; j<n; ++j)  {
	SL->A[i][j] = 1.0 / (real_t)(i+j+1);
      }
    }
  }
  else { // inicializa sistema normal e depois altera
    // inicializa a matriz A
    for (unsigned int i=0; i<n; ++i) {
      for (unsigned int j=0; j<n; ++j)  {
	SL->A[i][j] = (real_t)rand() * invRandMax;
      }
    }
    if (tipo == eqNula) {
      // sorteia eq a ser "nula"
      unsigned int nula = rand() % n;
      for (unsigned int j=0; j<n; ++j) {
	SL->A[nula][j] = 0.0;
      }
      SL->b[nula] = 0.0;
    } 
    else if (tipo == eqProporcional) {
      // sorteia eq a ser "proporcional" e valor
      unsigned int propDst = rand() % n;
      unsigned int propSrc = (propDst + 1) % n;
      real_t mult = (real_t)rand() * invRandMax;
      for (unsigned int j=0; j<n; ++j) {
	SL->A[propDst][j] = SL->A[propSrc][j] * mult;
      }
      SL->b[propDst] = SL->b[propSrc] * mult;
    } 
    else if (tipo == eqCombLinear) {
      // sorteia eq a ser "combLinear"
      unsigned int combDst = rand() % n;
      unsigned int combSrc1 = (combDst + 1) % n;
      unsigned int combSrc2 = (combDst + 2) % n;
      for (unsigned int j=0; j<n; ++j) {
	SL->A[combDst][j] = SL->A[combSrc1][j] + SL->A[combSrc2][j];
      }
      SL->b[combDst] = SL->b[combSrc1] + SL->b[combSrc2];
    }
    else if (tipo == diagDominante) {
      // aumenta o valor dos termos da diagonal principal
      for (unsigned int i=0; i<n; ++i) {
	real_t soma = 0.0;
	for (unsigned int j=0; j < i; ++j) soma += SL->A[i][j];
	for (unsigned int j=i+1; j < n; ++j) soma += SL->A[i][j];
        SL->A[i][i] += soma;
      }
    }
  }
}


SistLinear_t *lerSisLin (tipoAloc_t tipo)
{
  unsigned int n;
  SistLinear_t *SL;
  
  scanf("%d",&n);

  SL = alocaSisLin (n, tipo);
  
  for(int i=0; i < n; ++i)
    for(int j=0; j < n; ++j)
      scanf ("%lg", &SL->A[i][j]);

  for(int i=0; i < n; ++i)
    scanf ("%lg", &SL->b[i]);
  
  return SL;
}


void prnSisLin (SistLinear_t *SL)
{
  int n=SL->n;

  for(int i=0; i < n; ++i) {
    printf("\n  ");
    for(int j=0; j < n; ++j)
      printf ("%10g", SL->A[i][j]);
    printf ("   |   %g", SL->b[i]);
  }
  printf("\n\n");
}

void prnVetor (real_t *v, unsigned int n)
{
  int i;

  printf ("\n");
  for(i=0; i < n; ++i)
      printf ("%10g ", v[i]);
  printf ("\n\n");

}

