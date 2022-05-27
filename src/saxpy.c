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
    int processorID, masterProcessorID, tag, tmp = 0, start = 0;
    unsigned int arraySize, arraySizeLoc, remainder = 0, nProcessor = 0;
    float * a, * aLoc, * b, * bLoc, * c;
    float alpha;
    const int expectedArgc = 2;
    const char * expectedUsageMessage = "<configuration filepath>";
    char * outputFilePath = NULL;
    SAXPY_MODE saxpyMode;

    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage);
    setEnvironment(& a, & b, & alpha, & c, & arraySize, argv[1], & masterProcessorID, & nProcessor, 
                    & outputFilePath, (unsigned short int *) & saxpyMode);
    saxpy(a, b, & c, alpha, arraySize, saxpyMode);
    saveResult(c, arraySize, (const char *) outputFilePath);

    free(a);
    free(b);
    free(c);
    free(outputFilePath);
    return 0;
}