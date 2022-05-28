//
//  main_mpi.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 21/05/22.
//

/*
    Somma tra un vettore "a" moltiplicato per uno scalare "alpha" e un altro vettore "b" 
    in parallelo in ambiente MPI-Docker (c = alpha * a + b)
*/

#include "SaxpyLibrary.h"



int main (int argc, char ** argv) {
    const int expectedArgc = 2;
    const char * expectedUsageMessage = "<configuration filepath>";
    char * outputFilePath = NULL;

    int masterProcessorID;
    unsigned int arraySize;
    unsigned short int saxpyChosenMode;

    float * a, * b, * c;
    float alpha;
    
    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage);
    setEnvironment(& a, & b, & alpha, & c, & arraySize, argv[1], & masterProcessorID, & outputFilePath, & saxpyChosenMode);
    saxpy(a, b, & c, alpha, arraySize, saxpyChosenMode, masterProcessorID, & argc, & argv);
    saveResult(c, arraySize, (const char *) outputFilePath);

    free(a);
    free(b);
    free(c);
    free(outputFilePath);
    exit(0);
}