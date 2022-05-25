//
//  main_seq.c
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

// Somma tra un vettore "a" moltiplicato per uno scalare "alpha" e un altro vettore "b" in sequenziale (c = alpha * a + b)

#include "UsageUtility.h"



void executeComputation(float * a, float * b, float ** c, float alpha, unsigned int arraySize);

int main (int argc, char ** argv) {
    FILE * filePointer;
    int masterProcessorID;
    unsigned int arraySize, processorsAmount;
    float * a, * b, * c;
    float alpha;
    const int expectedArgc = 3;
    const char * expectedUsageMessage = "<configuration filepath> <output filepath>";
    char * outputFilePath = NULL;

    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage);
    setEnvironment(& a, & b, & alpha, & c, & arraySize, argv[1], & masterProcessorID, & processorsAmount);
    executeComputation(a, b, & c, alpha, arraySize);
    saveResult(c, arraySize, (const char *) argv[2]);

    free(a);
    free(b);
    free(c);
    return 0;
}

void executeComputation(float * a, float * b, float ** c, float alpha, unsigned int arraySize) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * a[i]) + b[i];
}
