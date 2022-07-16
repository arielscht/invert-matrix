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
  return((double)(tp.tv_sec*1.0e3 + tp.tv_nsec*1.0e-6));
}

/* Gera string '<baseName>_n'
 * Por exemplo, se baseName = "ABC" e n = 10,
 *  Função retorna a string "ABC_10"
 * Útil para gerar marcadores para LIKWID
 */
string_t markerName(string_t baseName, int n)
{
    string_t mark = (string_t) malloc( (strlen(baseName)+1) + numDigits(n) + 1 );

  sprintf(mark, "%s_%u", baseName,n);

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
