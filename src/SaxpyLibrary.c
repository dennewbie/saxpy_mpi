//
//  SaxpyLibrary.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "UsageUtility.h"



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, int masterProcessorID, MPI_Comm commWorld, int processorID, unsigned int nProcessor) {
    switch (saxpyMode) {
        case SAXPY_SEQUENTIAL:
            saxpy_sequential(a, b, c, alpha, arraySize, commWorld);
            break;
        case SAXPY_PARALLEL:
            saxpy_parallel(a, b, c, alpha, arraySize, masterProcessorID, commWorld, processorID, nProcessor);
            break;
        default:
            raiseError(INVALID_SAXPY_MODE_SCOPE, INVALID_SAXPY_MODE_ERROR, commWorld, FALSE);
            break;
    }
}

void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID, MPI_Comm commWorld, int processorID, unsigned int nProcessor) {
    int errorCode;
    int * recvcounts = NULL, * displacements = NULL, arraySizeLoc = 0;
    unsigned int remainder = 0, /*singleOffset = 0, */tag = 0/*, offset = 0*/;
    float * aLoc, * bLoc, * cLoc;
    aLoc = bLoc = cLoc = NULL;

    if ((errorCode = MPI_Bcast(& alpha, 1, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_BCAST_SCOPE, errorCode, commWorld, FALSE); 
    if ((errorCode = MPI_Barrier(commWorld) != MPI_SUCCESS)) raiseError(MPI_BARRIER_SCOPE, errorCode, commWorld, FALSE);

    arraySizeLoc = arraySize / nProcessor;
    remainder = arraySize % nProcessor;
    if (remainder > 0) {
        if (processorID < remainder) {
            arraySizeLoc += 1;
        /*} else {
            singleOffset = 1;*/
        }
    }

    if (processorID == masterProcessorID) {
        recvcounts = createIntArray(nProcessor, commWorld);
        displacements = createIntArray(nProcessor, commWorld);
        displacements[0] = 0;
    }

    if ((errorCode = MPI_Gather(& arraySizeLoc, 1, MPI_INT, recvcounts, 1, MPI_INT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_GATHER_SCOPE, errorCode, commWorld, FALSE);
    if (processorID == masterProcessorID) for (int i = 1; i < nProcessor; i++) displacements[i] = displacements[i - 1] + recvcounts[i - 1];

    aLoc = createFloatArray(arraySizeLoc, commWorld);
    bLoc = createFloatArray(arraySizeLoc, commWorld);
    cLoc = createFloatArray(arraySizeLoc, commWorld);
    // offset = (processorID * arraySizeLoc) + singleOffset;

    if ((errorCode = MPI_Scatterv(a, recvcounts, displacements, MPI_FLOAT, aLoc, arraySizeLoc, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_SCATTERV_SCOPE, errorCode, commWorld, FALSE); 
    if ((errorCode = MPI_Scatterv(b, recvcounts, displacements, MPI_FLOAT, bLoc, arraySizeLoc, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_SCATTERV_SCOPE, errorCode, commWorld, FALSE); 
    saxpy_sequential(aLoc, bLoc, & cLoc, alpha, arraySizeLoc, commWorld);
    if ((errorCode = MPI_Gatherv(cLoc, arraySizeLoc, MPI_FLOAT, * c, recvcounts, displacements, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_GATHERV_SCOPE, errorCode, commWorld, FALSE);
    
    releaseMemory(aLoc, bLoc, cLoc, (void *) 0);
    if (processorID == masterProcessorID) releaseMemory(recvcounts, displacements, (void *) 0);
}


void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, MPI_Comm commWorld) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * (* (a + i))) + (* (b + i));
}