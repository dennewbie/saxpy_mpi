//
//  SaxpyLibrary.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "SaxpyLibrary.h"



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, int masterProcessorID, int * argc, char *** argv) {
    switch (saxpyMode) {
        case SAXPY_SEQUENTIAL:
            saxpy_sequential(a, b, c, alpha, arraySize, masterProcessorID);
            break;
        case SAXPY_PARALLEL:
            saxpy_parallel(a, b, c, alpha, arraySize, masterProcessorID, argc, argv);
            break;
        default:
            raiseError(INVALID_SAXPY_MODE_SCOPE, INVALID_SAXPY_MODE_ERROR);
            break;
    }
}

void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID, int * argc, char *** argv) {
    int processorID, tag, tmp = 0, start = 0;
    unsigned int arraySizeLoc, remainder = 0, nProcessor = 0;
    float * aLoc, * bLoc;

    if (MPI_Init(argc, argv) != MPI_SUCCESS) raiseError(MPI_INIT_SCOPE, MPI_INIT_ERROR);
    if (MPI_Comm_rank(MPI_COMM_WORLD, & processorID) != MPI_SUCCESS) raiseError(MPI_COMM_RANK_SCOPE, MPI_COMM_RANK_ERROR);
    if (MPI_Comm_size(MPI_COMM_WORLD, & nProcessor) != MPI_SUCCESS) raiseError(MPI_COMM_SIZE_SCOPE, MPI_COMM_SIZE_ERROR);

    printf("\nSono %d\n", processorID);
    if (MPI_Finalize() != MPI_SUCCESS) raiseError(MPI_FINALIZE_SCOPE, MPI_FINALIZE_ERROR);
}


void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * (* (a + i))) + (* (b + i));
}