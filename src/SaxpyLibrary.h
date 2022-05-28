//
//  SaxpyLibrary.h
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

// #include "mpi.h"
#include "UsageUtility.h"



typedef enum {
    SAXPY_SEQUENTIAL = 0,
    SAXPY_PARALLEL = 1
} SAXPY_MODE;



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, int masterProcessorID);
void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID);
void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID);