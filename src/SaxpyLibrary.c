//
//  SaxpyLibrary.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "SaxpyLibrary.h"



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, int masterProcessorID) {
    switch (saxpyMode) {
        case SAXPY_SEQUENTIAL:
            saxpy_sequential(a, b, c, alpha, arraySize, masterProcessorID);
            break;
        case SAXPY_PARALLEL:
            saxpy_parallel(a, b, c, alpha, arraySize, masterProcessorID);
            break;
        default:
            raiseError(INVALID_SAXPY_MODE_SCOPE, INVALID_SAXPY_MODE_ERROR);
            break;
    }
}

void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID) {
    int processorID, tag, tmp = 0, start = 0;
    unsigned int arraySizeLoc, remainder = 0, nProcessor = 0;
    float * aLoc, * bLoc;
    const int expectedArgc = 2;
    const char * expectedUsageMessage = "<configuration filepath>";
    char * outputFilePath = NULL;
    SAXPY_MODE saxpyMode;
}


void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * (* (a + i))) + (* (b + i));
}