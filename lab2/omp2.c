/*
	Name: Lab#2
	Copyright: 2021-22
	Author: Prof. Joao Costa
	Date: 06/04/22 17:24
	Description: 
		Parallelizing an inner loop with dependences
	
			for (iter=0; iter<numiter; iter++) {
				for (i=0; i<size-1; i++) {
					V[i] = f( V[i], V[i+1] );
				}
			}
*/
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>   // ?? IMPORTANTE

#define TOTALSIZE 1000
#define NUMITER 200

#define f(x,y) ((x+y)/2.0)

int main(int argc, char *argv[]) {

  int i, iter;

  double *V = (double *) malloc(TOTALSIZE * sizeof(double));
  double *V_old = (double *) malloc(TOTALSIZE * sizeof(double)); // ? NOVO

  // 1. Inicializar vetor
  for(i = 0; i < TOTALSIZE; i++) {
    V[i] = 0.0 + i;
  }

  // 2. Iterações
  for(iter = 0; iter < NUMITER; iter++) {

    // ? COPIAR vetor antes de processar
    for(i = 0; i < TOTALSIZE; i++) {
      V_old[i] = V[i];
    }

    // ? Paralelizar corretamente
    #pragma omp parallel for
    for(i = 0; i < TOTALSIZE-1; i++) {
      V[i] = f(V_old[i], V_old[i+1]);
    }
  }

  // 3. Output
  printf("Output:\n"); 
  for(i = 0; i < TOTALSIZE; i++) {
    printf("%4d %f\n", i, V[i]);
  }

  return 0;
}
