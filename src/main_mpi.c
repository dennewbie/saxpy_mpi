//
//  main_mpi.c
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

// Somma tra un vettore "a" moltiplicato per uno scalare "alpha" e un altro vettore "b" in ambiente MPI-Docker (c = alpha * a + b)

#include "UsageUtility.h"



// void executeComputation(float * a, float * b, float ** c, float alpha, unsigned int arraySize);

int main (int argc, char ** argv) {
    /*
    FILE * filePointer;
    int processorID, masterProcessorID, tag, tmp = 0, start = 0;
    unsigned int arraySize, arraySizeLoc, remainder = 0, nProcessor;
    float * a, * aLoc, * b, * bLoc, * c;
    float alpha;
    const int expectedArgc = 3;
    const char * expectedUsageMessage = "<configuration filepath> <output filepath>";
    char * outputFilePath = NULL;

    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage);
    setEnvironment(& a, & b, & alpha, & c, argv[1], & masterProcessorID, & arraySize);
    executeComputation(a, b, & c, alpha, arraySize);
    saveResult(c, arraySize, (const char *) argv[2]);

    free(a);
    free(b);
    free(c);
    */
    return 0;
}
/*
void executeComputation(float * a, float * b, float ** c, float alpha, unsigned int arraySize) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * a[i]) + b[i];
}
*/