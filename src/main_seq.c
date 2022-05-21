//
//  main_seq.c
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

// Somma tra un vettore "a" moltiplicato per uno scalare "alpha" e un altro vettore "b" in sequenziale (c = alpha * a + b)

#include "UsageUtility.h"



int main (int argc, char * argv[]) {
    FILE * filePointer;
    int processorID, nProcessor, masterProcessorID, n, nLoc, remainder, tag, i, tmp = 0, start = 0;
    float * a, * aLoc, * b, * bLoc;
    float alpha, sum = 0.0, sumtot = 0.0;
    const int expectedArgc = 2;
    const char * expectedUsageMessage = "<configuration filepath>";

    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage);
    setEnvironment(& a, & b, & alpha, argv[1], & masterProcessorID);

    return 0;
}
