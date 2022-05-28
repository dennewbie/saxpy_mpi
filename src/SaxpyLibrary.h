//
//  SaxpyLibrary.h
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "mpi.h"
#include "UsageUtility.h"

// Error code and scope message
static const int MPI_INIT_ERROR = 101;
static const char * MPI_INIT_SCOPE = "MPI Init";

static const int MPI_FINALIZE_ERROR = 102;
static const char * MPI_FINALIZE_SCOPE = "MPI Finalize";

static const int MPI_COMM_RANK_ERROR = 103;
static const char * MPI_COMM_RANK_SCOPE = "MPI CommRank";

static const int MPI_COMM_SIZE_ERROR = 104;
static const char * MPI_COMM_SIZE_SCOPE = "MPI CommSize";

static const int MPI_SEND_ERROR = 105;
static const char * MPI_SEND_SCOPE = "MPI Send";

static const int MPI_RECV_ERROR = 106;
static const char * MPI_RECV_SCOPE = "MPI Recv";



static const int TAG_START_OFFSET = 10;



typedef enum {
    SAXPY_SEQUENTIAL = 0,
    SAXPY_PARALLEL = 1
} SAXPY_MODE;



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, int masterProcessorID, int * argc, char *** argv, const char * outputFilePath);
void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID, int * argc, char *** argv, const char * outputFilePath);
void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, const char * outputFilePath);