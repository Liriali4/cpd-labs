#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 100000

int main(int argc, char *argv[]) {

    int id, p, i;
    int buffer[SIZE];
    MPI_Status status;
    double start, end;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    start = MPI_Wtime();

    if(id == 0) {

        for(i = 1; i < p; i++) {
            MPI_Send(buffer, SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

    } else {

        MPI_Recv(buffer, SIZE, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, &status);
    }

    end = MPI_Wtime();

    if(id == 0) {

        double time = end - start;

        double bytes =
            SIZE * sizeof(int);

        double bandwidth =
            bytes / time / (1024 * 1024);

        printf("Tempo total: %f segundos\n", time);

        printf("Bandwidth: %f MB/s\n",
               bandwidth);
    }

    MPI_Finalize();

    return 0;
}