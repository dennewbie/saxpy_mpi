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

/*  TODO: 
    1) Usare MPI_Abort(...) DONE
    2) lettura file dopo mpi init e poi invio porzioni
    3) tempi
    4) nproc > elementi
    5) verifica con numero di elementi differenti tra i due array
*/

void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID, MPI_Comm commWorld, int processorID, unsigned int nProcessor) {
    int * recvcounts = NULL, * displacements = NULL, arraySizeLoc = 0;
    unsigned int remainder = 0, singleOffset = 0, tag = 0, offset = 0;
    float * aLoc, * bLoc, * cLoc;
    aLoc = bLoc = cLoc = NULL;

    MPI_Bcast(& alpha, 1, MPI_FLOAT, masterProcessorID, commWorld);

    arraySizeLoc = arraySize / nProcessor;
    remainder = arraySize % nProcessor;
    if (remainder > 0) {
        if (processorID < remainder) {
            arraySizeLoc += 1;
        } else {
            singleOffset = 1;
        }
    }

    if (processorID == masterProcessorID) {
        recvcounts = createIntArray(nProcessor, commWorld);
        displacements = createIntArray(nProcessor, commWorld);
        displacements[0] = 0;
    }

    if (MPI_Gather(& arraySizeLoc, 1, MPI_INT, recvcounts, 1, MPI_INT, masterProcessorID, commWorld) != MPI_SUCCESS) raiseError(MPI_GATHER_SCOPE, MPI_GATHER_ERROR, commWorld, FALSE);
    if (processorID == masterProcessorID) for (int i = 1; i < nProcessor; i++) displacements[i] = displacements[i - 1] + recvcounts[i - 1];

    aLoc = createFloatArray(arraySizeLoc, commWorld);
    bLoc = createFloatArray(arraySizeLoc, commWorld);
    cLoc = createFloatArray(arraySizeLoc, commWorld);
    offset = (processorID * arraySizeLoc) + singleOffset;

    // err hand
    MPI_Scatterv(a, recvcounts, displacements, MPI_FLOAT, aLoc, arraySizeLoc, MPI_FLOAT, masterProcessorID, commWorld);
    MPI_Scatterv(b, recvcounts, displacements, MPI_FLOAT, bLoc, arraySizeLoc, MPI_FLOAT, masterProcessorID, commWorld);

    saxpy_sequential(aLoc, bLoc, & cLoc, alpha, arraySizeLoc, commWorld);

    // deallocare memoria altri processori aLoc, bLoc, cLoc
    if (MPI_Gatherv(cLoc, arraySizeLoc, MPI_FLOAT, * c, recvcounts, displacements, MPI_FLOAT, masterProcessorID, commWorld) != MPI_SUCCESS) raiseError(MPI_GATHERV_SCOPE, MPI_GATHERV_ERROR, commWorld, FALSE);

    free(aLoc);
    free(bLoc);
    free(cLoc);
    if (processorID == masterProcessorID) free(recvcounts); free(displacements);
    // releaseMemory(aLoc, bLoc, cLoc);
    // if (processorID == masterProcessorID) releaseMemory(recvcounts, displacements);
}


void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, MPI_Comm commWorld) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * (* (a + i))) + (* (b + i));
}


/*
    if (processorID == masterProcessorID) {
        arraySizeLoc = arraySize / nProcessor;
        remainder = arraySize % nProcessor;

        for (int i = 0; i < nProcessor; i++) {
            tag = TAG_START_OFFSET + i;
            if (MPI_Send(& alpha, 1, MPI_FLOAT, tag, commWorld) != MPI_SUCCESS)  raiseError(MPI_SEND_SCOPE, MPI_SEND_ERROR, commWorld, FALSE);
            if (processorID < remainder) offset = TRUE;

            
            // spedire aLoc, bLoc, alfa
        }
    } else {
        tag = TAG_START_OFFSET + processorID;
        MPI_Recv(& alfa, 1, MPI_INT, 0, tag, commWorld, & status);
    }
    printf("\nSono %d\n", processorID);
    */


   // saxpy_sequential(a + offset, b + offset, & cLoc, alpha, arraySizeLoc, NULL, commWorld);