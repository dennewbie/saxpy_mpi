//
//  UsageUtility.c
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

#include "UsageUtility.h"



// check parameter/parameters passed via terminal
void checkUsage (int argc, const char ** argv, int expected_argc, const char * expectedUsageMessage, MPI_Comm commWorld) {
    if (argc != expected_argc) {
        if (fprintf(stderr, (const char * restrict) "Usage: %s %s\n", argv[0], expectedUsageMessage) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR, commWorld, FALSE);
        raiseError(CHECK_USAGE_SCOPE, CHECK_USAGE_ERROR, commWorld, FALSE);
    }
}

// Handle most common errors and show scope plus error code. After doing this, exit from process
void raiseError (const char * errorScope, int exitCode, MPI_Comm commWorld, boolean recursionOverflow) {
    if (recursionOverflow) {
        if (fprintf(stderr, (const char * restrict) "Scope: %s - Error #%d\n", RECURSION_OVERFLOW_SCOPE, RECURSION_OVERFLOW_ERROR) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR, commWorld, TRUE);
        exit(RECURSION_OVERFLOW_ERROR);
    } else {
        if (fprintf(stderr, (const char * restrict) "Scope: %s - Error #%d\n", errorScope, exitCode) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR, commWorld, FALSE);
        if (MPI_Abort(commWorld, exitCode) != MPI_SUCCESS) raiseError(MPI_ABORT_SCOPE, MPI_ABORT_ERROR, commWorld, TRUE);
    }
}

void setEnvironment (float ** a, float ** b, float * alpha, float ** c, unsigned int * arraySize, const char * configurationFilePath, char ** outputFilePathString, unsigned short int * saxpyMode, MPI_Comm commWorld) {
    FILE * configurationFilePointer, * dataFilePointer;
    ssize_t getLineBytes;
    size_t dataFilePathLength = 0, nLength = 0, singleNumberLength = 0, outputFilePathLength = 0, saxpyModeLength = 0;
    char * dataFilePathString = NULL, * nString = NULL, * singleNumberString = NULL, * saxpyModeString = NULL;
    float singleNumber = 0.0F;

    // read basic settings parameter from .config file
    configurationFilePointer = fopen(configurationFilePath, "r");
    if (!configurationFilePointer) raiseError(CONFIGURATION_FILE_OPEN_SCOPE, CONFIGURATION_FILE_OPEN_ERROR, commWorld, FALSE);
    // read saxpy approach
    if ((getLineBytes = getline((char ** restrict) & saxpyModeString, (size_t * restrict) & saxpyModeLength, (FILE * restrict) configurationFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR, commWorld, FALSE);
    * saxpyMode = (unsigned short int) strtoul((const char * restrict) saxpyModeString, (char ** restrict) NULL, 10);
    if (* saxpyMode == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOUL_SCOPE, STRTOUL_ERROR, commWorld, FALSE);
    // read input data file path
    if ((getLineBytes = getline((char ** restrict) & dataFilePathString, (size_t * restrict) & dataFilePathLength, (FILE * restrict) configurationFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR, commWorld, FALSE);
    dataFilePathString[strlen(dataFilePathString) - 1] = '\0';
    // read output data file path
    if ((getLineBytes = getline((char ** restrict) outputFilePathString, (size_t * restrict) & outputFilePathLength, (FILE * restrict) configurationFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR, commWorld, FALSE);

    // read amount of numbers to read for each array
    dataFilePointer = fopen(dataFilePathString, "r");
    if (!dataFilePointer) raiseError(DATA_FILE_OPEN_SCOPE, DATA_FILE_OPEN_ERROR, commWorld, FALSE);
    if ((getLineBytes = getline((char ** restrict) & nString, (size_t * restrict) & nLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR, commWorld, FALSE);
    * arraySize = (unsigned int) strtoul((const char * restrict) nString, (char ** restrict) NULL, 10);
    if (* arraySize == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOUL_SCOPE, STRTOUL_ERROR, commWorld, FALSE);
    if (* arraySize <= 0) raiseError(INVALID_ARRAY_SIZE_SCOPE, INVALID_ARRAY_SIZE_ERROR, commWorld, FALSE);

    // read array a, b and scalar alpha from file. Create array c
    createFloatArrayFromFile(dataFilePointer, a, * arraySize, commWorld);
    createFloatArrayFromFile(dataFilePointer, b, * arraySize, commWorld);
    if ((getLineBytes = getline((char ** restrict) & singleNumberString, (size_t * restrict) & singleNumberLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR, commWorld, FALSE);
    * alpha = (float) strtof((const char *) singleNumberString, (char ** restrict) NULL);
    if ((* alpha == 0.0F || * alpha == HUGE_VALF) && (errno == ERANGE)) raiseError(STRTOF_SCOPE, STRTOF_ERROR, commWorld, FALSE);

    * c = createFloatArray(* arraySize, commWorld);

    closeFiles(configurationFilePointer, dataFilePointer, (void *) 0);
    releaseMemory(dataFilePathString, nString, singleNumberString, saxpyModeString, (void *) 0);
}

void createFloatArrayFromFile (FILE * filePointer, float ** array, unsigned int arraySize, MPI_Comm commWorld) {
    char * singleNumberString = NULL;
    size_t singleNumberLength = 0;
    ssize_t getLineBytes;
    float singleNumber = 0.0F;

    * array = createFloatArray(arraySize, commWorld);
    for (int i = 0; i < arraySize; i++) {
        if ((getLineBytes = getline((char ** restrict) & singleNumberString, (size_t * restrict) & singleNumberLength, (FILE * restrict) filePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR, commWorld, FALSE);
        singleNumber = (float) strtof((const char *) singleNumberString, (char ** restrict) NULL);
        if ((singleNumber == 0.0F || singleNumber == HUGE_VALF) && (errno == ERANGE)) raiseError(STRTOF_SCOPE, STRTOF_ERROR, commWorld, FALSE);
        *((* array) + i) = singleNumber;
    }

    releaseMemory(singleNumberString, (void *) 0);
}

// just print the array on file pointer
void printArray (FILE * filePointer, float * array, unsigned int arraySize, MPI_Comm commWorld) {
    for (int i = 0; i < arraySize; i++) if (fprintf(filePointer, "%.5f\n", array[i]) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR, commWorld, FALSE);
}

void saveResult (float * array, unsigned int arraySize, const char * outputFilePath, MPI_Comm commWorld) {
    FILE * outputFilePointer = fopen(outputFilePath, "w");
    if (!outputFilePointer) raiseError(DATA_FILE_OPEN_SCOPE, DATA_FILE_OPEN_ERROR, commWorld, FALSE);
    printArray(outputFilePointer, array, arraySize, commWorld);
    closeFiles(outputFilePointer, (void *) 0);
}

float * createFloatArray (unsigned int arraySize, MPI_Comm commWorld) {
    float * array = (float *) calloc(arraySize, sizeof(* array));
    if (!array) raiseError(CALLOC_SCOPE, CALLOC_ERROR, commWorld, FALSE);
    return array;
}

int * createIntArray (unsigned int arraySize, MPI_Comm commWorld) {
    int * array = (int *) calloc(arraySize, sizeof(* array));
    if (!array) raiseError(CALLOC_SCOPE, CALLOC_ERROR, commWorld, FALSE);
    return array;
}

void releaseMemory (void * arg1, ... ) {
    va_list argumentsList;
    void * currentElement;
    free(arg1);
    va_start(argumentsList, arg1);
    while ((arg1 = va_arg(argumentsList, void *)) != 0) free(arg1);
    va_end(argumentsList);
}

void closeFiles (void * arg1, ... ) {
    va_list argumentsList;
    void * currentElement;
    fclose(arg1);
    va_start(argumentsList, arg1);
    while ((arg1 = va_arg(argumentsList, void *)) != 0) fclose(arg1);
    va_end(argumentsList);
}