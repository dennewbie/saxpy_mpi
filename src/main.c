// Somma tra un vettore a moltiplicato per uno scalare alpha e un altro vettore b in ambiente MPI-Docker (c = alpha * a + b)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mpi.h"

int main (int argc, char * argv[]) {
    FILE * filePointer;
    int menum, nproc;
    int n, nloc, tag, i, rest, tmp = 0, start = 0, sum = 0, sumtot = 0;
    int * x, * xloc;
    MPI_Status status;

    MPI_Init(& argc, & argv);
    MPI_Comm_rank(MPI_COMM_WORLD, & menum); 
    MPI_Comm_size(MPI_COMM_WORLD, & nproc);

    MPI_Finalize();
    return 0;
}
