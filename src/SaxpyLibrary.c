//
//  SaxpyLibrary.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "UsageUtility.h"



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, int masterProcessorID, int * argc, char *** argv, const char * outputFilePath) {
    switch (saxpyMode) {
        case SAXPY_SEQUENTIAL:
            saxpy_sequential(a, b, c, alpha, arraySize, NULL);
            break;
        case SAXPY_PARALLEL:
            saxpy_parallel(a, b, c, alpha, arraySize, masterProcessorID, argc, argv, outputFilePath);
            break;
        default:
            raiseError(INVALID_SAXPY_MODE_SCOPE, INVALID_SAXPY_MODE_ERROR);
            break;
    }
}

/*  TODO: 
    1) Usare MPI_Abort(...)
    2) lettura file dopo mpi init e poi invio porzioni
    3) tempi
*/

void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID, int * argc, char *** argv, const char * outputFilePath) {
    int processorID, * recvcounts = NULL, * displacements = NULL;
    unsigned int arraySizeLoc = 0, remainder = 0, nProcessor = 0, singleOffset = 0, tag = 0, tmp = 0, offset = 0;
    float * aLoc, * bLoc;

    if (MPI_Init(argc, argv) != MPI_SUCCESS) raiseError(MPI_INIT_SCOPE, MPI_INIT_ERROR);
    if (MPI_Comm_rank(MPI_COMM_WORLD, & processorID) != MPI_SUCCESS) raiseError(MPI_COMM_RANK_SCOPE, MPI_COMM_RANK_ERROR);
    if (MPI_Comm_size(MPI_COMM_WORLD, & nProcessor) != MPI_SUCCESS) raiseError(MPI_COMM_SIZE_SCOPE, MPI_COMM_SIZE_ERROR);

    arraySizeLoc = arraySize / nProcessor;
    remainder = arraySize % nProcessor;
    if (remainder > 0) {
        if (processorID < remainder) {
            arraySizeLoc += 1;
        } else {
            singleOffset = 1;
        }
    }

    float * cLoc = (float *) calloc(arraySizeLoc, sizeof(* cLoc));
    if (!cLoc) raiseError(CALLOC_SCOPE, CALLOC_ERROR);
    
    offset = (processorID * arraySizeLoc) + singleOffset;
    saxpy_sequential(a + offset, b + offset, & cLoc, alpha, arraySizeLoc, NULL);

    if (processorID == masterProcessorID) {
        recvcounts = (int *) calloc(nProcessor, sizeof(* recvcounts));
        displacements = (int *) calloc(nProcessor, sizeof(* displacements));
        if (!recvcounts || !displacements) raiseError(CALLOC_SCOPE, CALLOC_ERROR);
        displacements[0] = 0;
    }

    if (MPI_Gather(& arraySizeLoc, 1, MPI_INT, recvcounts, 1, MPI_INT, masterProcessorID, MPI_COMM_WORLD) != MPI_SUCCESS) raiseError(MPI_GATHER_SCOPE, MPI_GATHER_ERROR);
    if (processorID == masterProcessorID) for (int i = 1; i < nProcessor; i++) displacements[i] = displacements[i - 1] + recvcounts[i - 1];

    if (MPI_Gatherv(cLoc, arraySizeLoc, MPI_FLOAT, * c, recvcounts, displacements, MPI_FLOAT, masterProcessorID, MPI_COMM_WORLD) != MPI_SUCCESS) raiseError(MPI_GATHERV_SCOPE, MPI_GATHERV_ERROR);
    if (processorID == masterProcessorID && outputFilePath != NULL) saveResult(* c, arraySize, outputFilePath);
    if (MPI_Finalize() != MPI_SUCCESS) raiseError(MPI_FINALIZE_SCOPE, MPI_FINALIZE_ERROR);
}


void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, const char * outputFilePath) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * (* (a + i))) + (* (b + i));
    if (outputFilePath != NULL) saveResult(* c, arraySize, outputFilePath);
}


/*
    if (processorID == masterProcessorID) {
        arraySizeLoc = arraySize / nProcessor;
        remainder = arraySize % nProcessor;

        for (int i = 0; i < nProcessor; i++) {
            tag = TAG_START_OFFSET + i;
            if (MPI_Send(& alpha, 1, MPI_FLOAT, tag, MPI_COMM_WORLD) != MPI_SUCCESS)  raiseError(MPI_SEND_SCOPE, MPI_SEND_ERROR);
            if (processorID < remainder) offset = TRUE;

            
            // spedire aLoc, bLoc, alfa
        }
    } else {
        tag = TAG_START_OFFSET + processorID;
        MPI_Recv(& alfa, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, & status);
    }
    printf("\nSono %d\n", processorID);
    */