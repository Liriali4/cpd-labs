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
#include<omp.h>

#define TOTALSIZE 1000
#define NUMITER 200

#define f(x,y) ((x+y)/2.0)

int main(int argc, char *argv[]) {

  int i, iter;

  double *V = (double *) malloc(TOTALSIZE * sizeof(double));
  double *V_new = (double *) malloc(TOTALSIZE * sizeof(double)); // segundo vetor

  // 1. Inicializar
  for(i = 0; i < TOTALSIZE; i++) {
    V[i] = 0.0 + i;
  }

  // 2. Iterações
  for(iter = 0; iter < NUMITER; iter++) {

    #pragma omp parallel for
    for(i = 0; i < TOTALSIZE-1; i++) {
      V_new[i] = f(V[i], V[i+1]); // usa V, escreve em V_new
    }

    // IMPORTANTE: último elemento
    V_new[TOTALSIZE-1] = V[TOTALSIZE-1];

    // Trocar vetores
    double *temp = V;
    V = V_new;
    V_new = temp;
  }

  // 3. Output
  printf("Output:\n"); 
  for(i = 0; i < TOTALSIZE; i++) {
    printf("%4d %f\n", i, V[i]);
  }

  return 0;
}
