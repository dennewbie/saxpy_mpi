//
//  UsageUtility.c
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

#include "UsageUtility.h"



// check parameter/parameters passed via terminal
void checkUsage (int argc, const char * argv[], int expected_argc, const char * expectedUsageMessage) {
    if (argc != expected_argc) {
        if (fprintf(stderr, (const char * restrict) "Usage: %s %s\n", argv[0], expectedUsageMessage) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR);
        raiseError(CHECK_USAGE_SCOPE, CHECK_USAGE_ERROR);
    }
}

// Handle most common errors and show scope plus error code. After doing this, exit from process
void raiseError (const char * errorScope, int exitCode) {
    if (fprintf(stderr, (const char * restrict) "Scope: %s - Error #%d\n", errorScope, exitCode) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR);
    exit(exitCode);
}

void setEnvironment (float ** a, float ** b, float * alpha, float ** c, const char * configurationFilePath, int * masterProcessorID, unsigned int * arraySize) {
    FILE * configurationFilePointer, * dataFilePointer;
    ssize_t getLineBytes;
    size_t masterProcessorID_length = 0, dataFilePathLength = 0, nLength = 0, singleNumberLength = 0;
    char * masterProcessorID_string = NULL, * dataFilePathString = NULL, * nString = NULL, * singleNumberString = NULL;
    float singleNumber = 0.0F;

    // read basic settings parameter from .config file
    configurationFilePointer = fopen(configurationFilePath, "r");
    if (!configurationFilePointer) raiseError(CONFIGURATION_FILE_OPEN_SCOPE, CONFIGURATION_FILE_OPEN_ERROR);
    if ((getLineBytes = getline((char ** restrict) & dataFilePathString, (size_t * restrict) & dataFilePathLength, (FILE * restrict) configurationFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    dataFilePathString[strlen(dataFilePathString) - 1] = '\0';
    if ((getLineBytes = getline((char ** restrict) & masterProcessorID_string, (size_t * restrict) & masterProcessorID_length, (FILE * restrict) configurationFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    * masterProcessorID = (int) strtoul((const char * restrict) masterProcessorID_string, (char ** restrict) NULL, 10);
    if (* masterProcessorID == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOUL_SCOPE, STRTOUL_ERROR);

    // read amount of numbers to read for each array
    dataFilePointer = fopen(dataFilePathString, "r");
    if (!dataFilePointer) raiseError(DATA_FILE_OPEN_SCOPE, DATA_FILE_OPEN_ERROR);
    if ((getLineBytes = getline((char ** restrict) & nString, (size_t * restrict) & nLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    * arraySize = (unsigned int) strtoul((const char * restrict) nString, (char ** restrict) NULL, 10);
    if (* arraySize == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOUL_SCOPE, STRTOUL_ERROR);

    // read array a, b and scalar alpha from file. Create array c
    createArrayWithNumbersFromFile(dataFilePointer, a, * arraySize);
    createArrayWithNumbersFromFile(dataFilePointer, b, * arraySize);
    if ((getLineBytes = getline((char ** restrict) & singleNumberString, (size_t * restrict) & singleNumberLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    * alpha = (float) strtof((const char *) singleNumberString, (char ** restrict) NULL);
    if ((* alpha == 0.0F || * alpha == HUGE_VALF) && (errno == ERANGE)) raiseError(STRTOF_SCOPE, STRTOF_ERROR);

    * c = (float *) calloc(* arraySize, sizeof(** c));
    if (!(* c)) raiseError(CALLOC_SCOPE, CALLOC_ERROR);
    
    // free up memory and close file pointer
    fclose(configurationFilePointer);
    fclose(dataFilePointer);
    free(masterProcessorID_string);
    free(dataFilePathString);
    free(nString);
    free(singleNumberString);
}

void createArrayWithNumbersFromFile (FILE * filePointer, float ** array, unsigned int arraySize) {
    char * singleNumberString = NULL;
    size_t singleNumberLength = 0;
    ssize_t getLineBytes;
    float singleNumber = 0.0F;

    * array = (float *) calloc(arraySize, sizeof(* array));
    if (!array) raiseError(CALLOC_SCOPE, CALLOC_ERROR);
    for (int i = 0; i < arraySize; i++) {
        if ((getLineBytes = getline((char ** restrict) & singleNumberString, (size_t * restrict) & singleNumberLength, (FILE * restrict) filePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
        singleNumber = (float) strtof((const char *) singleNumberString, (char ** restrict) NULL);
        if ((singleNumber == 0.0F || singleNumber == HUGE_VALF) && (errno == ERANGE)) raiseError(STRTOF_SCOPE, STRTOF_ERROR);
        *((* array) + i) = singleNumber;
    }

    free(singleNumberString);
}

// just print the array on stdout
void printArray (float * array, unsigned int arraySize) {
    if (fprintf(stdout, "\n\n") < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR);
    for (int i = 0; i < arraySize; i++) if (fprintf(stdout, "%.5f\n", array[i]) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR);
}

void saveResult (float * array, unsigned int arraySize, const char * outputFilePath) {
    FILE * outputFilePointer;
    outputFilePointer = fopen(outputFilePath, "w");
    if (!outputFilePointer) raiseError(DATA_FILE_OPEN_SCOPE, DATA_FILE_OPEN_ERROR);
    for (int i = 0; i < arraySize; i++) if (fprintf(outputFilePointer, "%.5f\n", array[i]) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR);
    fclose(outputFilePointer);
}