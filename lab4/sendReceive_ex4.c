#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 100000

int main(int argc, char *argv[]) {

    int id, p, i;
    int array[SIZE];
    MPI_Status status;

    double start, end;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    /* ENVIO INDIVIDUAL */

    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();

    if(id == 0) {

        for(i = 1; i < p; i++) {

            MPI_Send(array, SIZE,
                     MPI_INT, i,
                     0, MPI_COMM_WORLD);
        }

    } else {

        MPI_Recv(array, SIZE,
                 MPI_INT, 0,
                 0, MPI_COMM_WORLD,
                 &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    end = MPI_Wtime();

    if(id == 0) {

        printf("Tempo Send Individual: %f\n",
               end - start);
    }

    /* BROADCAST */

    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();

    MPI_Bcast(array, SIZE,
              MPI_INT, 0,
              MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    end = MPI_Wtime();

    if(id == 0) {

        printf("Tempo Broadcast: %f\n",
               end - start);
    }

    MPI_Finalize();

    return 0;
}
