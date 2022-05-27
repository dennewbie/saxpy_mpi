//
//  SaxpyLibrary.h
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

// #include "mpi.h"
#include "UsageUtility.h"



typedef enum {
    SEQUENTIAL_SAXPY = 0,
    PARALLEL_SAXPY = 1
} SAXPY_MODE;



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, SAXPY_MODE mode);