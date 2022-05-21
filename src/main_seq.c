//
//  main_seq.c
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

// Somma tra un vettore "a" moltiplicato per uno scalare "alpha" e un altro vettore "b" in sequenziale (c = alpha * a + b)

#include "UsageUtility.h"

unsigned short int MASTER_PROCESSOR_ID = 0;

int main (int argc, char * argv[]) {
    FILE * filePointer;
    int processorID, nProcessor;
    int n, nloc, tag, i, rest, tmp = 0, start = 0, sum = 0, sumtot = 0;
    int * x, * xloc;

    if (processorID == MASTER_PROCESSOR_ID) {
        filePointer = fopen("../data/inputData.dat", "r");
        if (!filePointer) {
            printf("\nErrore durante l'apertura del file contenente i dati di input\nExit...\n");
            printf("Error %d \n", errno);
            return(-2);
        }

        // P0 legge n, alloca x, legge x
        printf("\nLettura del size del problema (n)...\n");
        fscanf(filePointer, "%d", & n);
        // scanf(“%d”, & n);

        printf("\nLettura degli elementi del vettore di input...\n");
        x = (int *) calloc(n, sizeof(* x));
        if (!x) {
            printf("\nErrore durante l'allocazione del vettore x\nExit...\n");
            return(-1);
        }        

        for (i = 0; i < n; i++) fscanf(filePointer, "%d", & x[i]);
    }
    return 0;
}
