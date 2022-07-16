#ifndef __SISLIN_H__
#define __SISLIN_H__

#define COEF_MAX 32.0 // Valor máximo usado para gerar valores aleatórios de
		      // coeficientes nos sistemas lineares.

// Tipo de alocação para matrizes
typedef enum {
  pontPont=0, // Matriz como vetor de N ponteiros para vetores de tamanho N
  pontVet     // Matriz como vetor de N ponteiros para um único vetor de tamanho N*N
} tipoAloc_t;

// Estrutura para definiçao de um sistema linear qualquer
typedef struct {
  real_t **A; // coeficientes
  real_t *b; // termos independentes
  unsigned int n; // tamanho do SL
  tipoAloc_t tipoAloc_A; // tipo de alocação usada na matriz de coeficientes
} SistLinear_t;

// Tipos de matrizes de coeficientes usados pela função 'inicializaSistLinear()'
typedef enum {
    generico = 0,
    hilbert,
    diagDominante,
    eqNula,
    eqProporcional,
    eqCombLinear
} tipoSistLinear_t;


// Alocaçao e desalocação de matrizes
SistLinear_t* alocaSisLin (unsigned int n, tipoAloc_t tipo);
void liberaSisLin (SistLinear_t *SL);
void iniSisLin (SistLinear_t *SL, tipoSistLinear_t tipo, real_t coef_max);

// Leitura e impressão de sistemas lineares
SistLinear_t *lerSisLin ();
void prnSisLin (SistLinear_t *SL);
void prnVetor (real_t *vet, unsigned int n);

#endif // __SISLIN_H__

