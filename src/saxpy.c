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

#include "UsageUtility.h"



int main (int argc, char ** argv) {
    const int expectedArgc = 3;
    const char * expectedUsageMessage = "<configuration filepath> <master processor ID>";
    char * outputFilePath = NULL;

    int masterProcessorID, processorID;
    unsigned int arraySize = 0,  nProcessor = 0;
    unsigned short int saxpyChosenMode;

    float * a, * b, * c, alpha;
    a = b = c = NULL;
    MPI_Comm myCommWorld = MPI_COMM_WORLD;

    if (MPI_Init(& argc, & argv) != MPI_SUCCESS) raiseError(MPI_INIT_SCOPE, MPI_INIT_ERROR, myCommWorld, FALSE);
    if (MPI_Comm_rank(myCommWorld, & processorID) != MPI_SUCCESS) raiseError(MPI_COMM_RANK_SCOPE, MPI_COMM_RANK_ERROR, myCommWorld, FALSE);
    if (MPI_Comm_size(myCommWorld, & nProcessor) != MPI_SUCCESS) raiseError(MPI_COMM_SIZE_SCOPE, MPI_COMM_SIZE_ERROR, myCommWorld, FALSE);
    
    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage, myCommWorld);
    masterProcessorID = (int) strtol((const char * restrict) argv[2], (char ** restrict) NULL, 10);
    if (masterProcessorID == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOL_SCOPE, STRTOL_ERROR, myCommWorld, FALSE);
    if (processorID == masterProcessorID) setEnvironment(& a, & b, & alpha, & c, & arraySize, argv[1], & outputFilePath, & saxpyChosenMode, myCommWorld);

    MPI_Bcast(& masterProcessorID, 1, MPI_INT, masterProcessorID, myCommWorld);
    MPI_Bcast(& saxpyChosenMode, 1, MPI_UNSIGNED_SHORT, masterProcessorID, myCommWorld);
    MPI_Bcast(& arraySize, 1, MPI_UNSIGNED, masterProcessorID, myCommWorld);
    
    if (MPI_Barrier(myCommWorld) != MPI_SUCCESS) raiseError(MPI_BARRIER_SCOPE, MPI_BARRIER_ERROR, myCommWorld, FALSE);
    // saxpy(a, b, & c, alpha, arraySize, saxpyChosenMode, masterProcessorID, myCommWorld, processorID, nProcessor);
    
    if (processorID == masterProcessorID) {
        saveResult(c, arraySize, outputFilePath, myCommWorld);
        free(a);
        free(b);
        free(c);
        free(outputFilePath);
    }
    
    if (MPI_Finalize() != MPI_SUCCESS) raiseError(MPI_FINALIZE_SCOPE, MPI_FINALIZE_ERROR, myCommWorld, FALSE);
    exit(0);
}