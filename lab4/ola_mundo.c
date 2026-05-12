#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int id, p, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    MPI_Get_processor_name(hostname, &len);

    printf("O Processo %d envia cumprimentos a partir da máquina %s, com %d processos!\n",
           id, hostname, p);

    MPI_Finalize();

    return 0;
}









