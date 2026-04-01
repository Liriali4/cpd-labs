CC = gcc
CFLAGS = -fopenmp -Wall

all: omp1 omp2

omp1: omp1.c
	$(CC) $(CFLAGS) omp1.c -o omp1

omp2: omp2.c
	$(CC) $(CFLAGS) omp2.c -o omp2

run2:
	OMP_NUM_THREADS=2 ./omp1

run4:
	OMP_NUM_THREADS=4 ./omp1

run2_omp2:
	OMP_NUM_THREADS=2 ./omp2

run4_omp2:
	OMP_NUM_THREADS=4 ./omp2

clean:
	rm -f omp1 omp2