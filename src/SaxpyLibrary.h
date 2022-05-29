//
//  SaxpyLibrary.h
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "mpi.h"
#include <stdio.h>
// fprintf, perror
#include <stdlib.h>
// exit
#include <errno.h>
#include <string.h>
// strlen
#include <time.h>
// time
#include <math.h>
// HUGE_VALF
#include <stdarg.h>
// for variadic procedures



// Error code and scope message
static const char * MPI_INIT_SCOPE = "MPI Init";
static const char * MPI_FINALIZE_SCOPE = "MPI Finalize";
static const char * MPI_COMM_RANK_SCOPE = "MPI CommRank";
static const char * MPI_COMM_SIZE_SCOPE = "MPI CommSize";
static const char * MPI_SEND_SCOPE = "MPI Send";
static const char * MPI_RECV_SCOPE = "MPI Recv";
static const char * MPI_GATHER_SCOPE = "MPI Gather";
static const char * MPI_GATHERV_SCOPE = "MPI Gatherv";
static const char * MPI_BARRIER_SCOPE = "MPI Barrier";
static const char * MPI_BCAST_SCOPE = "MPI Bcast";
static const char * MPI_SCATTER_SCOPE = "MPI Scatter";
static const char * MPI_SCATTERV_SCOPE = "MPI Scatterv";

static const int MPI_ABORT_ERROR = 101;
static const char * MPI_ABORT_SCOPE = "MPI Abort";

static const int INVALID_ARRAY_SIZE_ERROR = 102;
static const char * INVALID_ARRAY_SIZE_SCOPE = "setEnvironment/reading array size: invalid array size. It should be greater than zero";



typedef enum {
    SAXPY_SEQUENTIAL = 0,
    SAXPY_PARALLEL = 1
} SAXPY_MODE;



void saxpy              (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, 
                        int masterProcessorID, MPI_Comm commWorld, int processorID, unsigned int nProcessor);

void saxpy_parallel     (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID, 
                        MPI_Comm commWorld, int processorID, unsigned int nProcessor);

void saxpy_sequential   (float * a, float * b, float ** c, float alpha, unsigned int arraySize, MPI_Comm commWorld);