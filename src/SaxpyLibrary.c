//
//  SaxpyLibrary.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "SaxpyLibrary.h"



void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, SAXPY_MODE mode) {
    switch (mode) {
    case SEQUENTIAL_SAXPY:
        for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * a[i]) + b[i];
        break;
    case PARALLEL_SAXPY:
        // stuff
        break;
    default:
        raiseError(INVALID_SAXPY_MODE_SCOPE, INVALID_SAXPY_MODE_ERROR);
        break;
    }
}