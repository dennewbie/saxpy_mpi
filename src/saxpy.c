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

    int masterProcessorID, processorID, errorCode;
    unsigned int arraySize = 0,  nProcessor = 0;
    unsigned short int saxpyChosenMode;

    float * a, * b, * c, alpha;
    double startTime, endTime, totalTime, maxTime;
    a = b = c = NULL;
    MPI_Comm myCommWorld = MPI_COMM_WORLD;

    if ((errorCode = MPI_Init(& argc, & argv)) != MPI_SUCCESS) raiseError(MPI_INIT_SCOPE, errorCode, myCommWorld, FALSE);
    if ((errorCode = MPI_Comm_rank(myCommWorld, & processorID)) != MPI_SUCCESS) raiseError(MPI_COMM_RANK_SCOPE, errorCode, myCommWorld, FALSE);
    if ((errorCode = MPI_Comm_size(myCommWorld, & nProcessor)) != MPI_SUCCESS) raiseError(MPI_COMM_SIZE_SCOPE, errorCode, myCommWorld, FALSE);
    
    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage, myCommWorld);
    masterProcessorID = (int) strtol((const char * restrict) argv[2], (char ** restrict) NULL, 10);
    if (masterProcessorID == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOL_SCOPE, STRTOL_ERROR, myCommWorld, FALSE);
    if (processorID == masterProcessorID) setEnvironment(& a, & b, & alpha, & c, & arraySize, argv[1], & outputFilePath, & saxpyChosenMode, myCommWorld);
    
    if ((errorCode = MPI_Barrier(myCommWorld) != MPI_SUCCESS)) raiseError(MPI_BARRIER_SCOPE, errorCode, myCommWorld, FALSE);
    startTime = MPI_Wtime();

    if ((errorCode = MPI_Bcast(& saxpyChosenMode, 1, MPI_UNSIGNED_SHORT, masterProcessorID, myCommWorld)) != MPI_SUCCESS) raiseError(MPI_BCAST_SCOPE, errorCode, myCommWorld, FALSE); 
    if ((errorCode = MPI_Bcast(& arraySize, 1, MPI_UNSIGNED, masterProcessorID, myCommWorld)) != MPI_SUCCESS) raiseError(MPI_BCAST_SCOPE, errorCode, myCommWorld, FALSE); 
    
    saxpy(a, b, & c, alpha, arraySize, saxpyChosenMode, masterProcessorID, myCommWorld, processorID, nProcessor);
    
    if ((errorCode = MPI_Barrier(myCommWorld) != MPI_SUCCESS)) raiseError(MPI_BARRIER_SCOPE, errorCode, myCommWorld, FALSE);
    endTime = MPI_Wtime();
    totalTime = endTime - startTime;
    MPI_Reduce(& totalTime, & maxTime, 1, MPI_DOUBLE, MPI_MAX, masterProcessorID, myCommWorld);
    if (processorID == masterProcessorID) {
        if (fprintf(stdout, (const char * restrict) OUTPUT_USER_MESSAGE, outputFilePath, arraySize, alpha, maxTime) < 0) {
            raiseError(FPRINTF_SCOPE, FPRINTF_ERROR, myCommWorld, FALSE);
        }

        saveResult(c, arraySize, outputFilePath, myCommWorld);
        releaseMemory(a, b, c, outputFilePath, (void *) 0);
    }
    
    if ((errorCode = MPI_Finalize() != MPI_SUCCESS)) raiseError(MPI_FINALIZE_SCOPE, errorCode, myCommWorld, FALSE);
    exit(0);
}