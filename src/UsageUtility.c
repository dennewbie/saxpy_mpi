//
//  UsageUtility.c
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

#include "UsageUtility.h"



// Controllo dei parametri passati da linea di comando
void checkUsage (int argc, const char * argv[], int expected_argc, const char * expectedUsageMessage) {
    if (argc != expected_argc) {
        if (fprintf(stderr, (const char * restrict) "Usage: %s %s\n", argv[0], expectedUsageMessage) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR);
        raiseError(CHECK_USAGE_SCOPE, CHECK_USAGE_ERROR);
    }
}

// Procedura per la gestione degli errori più comuni con la visualizzazione dello scope e conseguente terminazione del processo.
void raiseError (const char * errorScope, int exitCode) {
    if (fprintf(stderr, (const char * restrict) "Scope: %s - Error #%d\n", errorScope, exitCode) < 0) raiseError(FPRINTF_SCOPE, FPRINTF_ERROR);
    exit(exitCode);
}

void setEnvironment (float ** a, float ** b, float * alpha, const char * configurationFilePath, int * masterProcessorID) {
    FILE * configurationFilePointer, * dataFilePointer;
    ssize_t getLineBytes;
    size_t masterProcessorID_length = 0, dataFilePathLength = 0, nLength = 0, singleNumberLength = 0;
    char * masterProcessorID_string = NULL, * dataFilePathString = NULL, * nString = NULL, * singleNumberString = NULL;
    unsigned int n;
    float singleNumber;

    configurationFilePointer = fopen(configurationFilePath, "r");
    if (!configurationFilePointer) raiseError(CONFIGURATION_FILE_OPEN_SCOPE, CONFIGURATION_FILE_OPEN_ERROR);
    if ((getLineBytes = getline((char ** restrict) & dataFilePathString, (size_t * restrict) & dataFilePathLength, (FILE * restrict) configurationFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    if ((getLineBytes = getline((char ** restrict) & masterProcessorID_string, (size_t * restrict) & masterProcessorID_length, (FILE * restrict) configurationFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    * masterProcessorID = (int) strtoul((const char * restrict) masterProcessorID_string, (char ** restrict) NULL, 10);
    if (* masterProcessorID == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOUL_SCOPE, STRTOUL_ERROR);

    dataFilePointer = fopen(dataFilePathString, "r");
    if (!dataFilePointer) raiseError(DATA_FILE_OPEN_SCOPE, DATA_FILE_OPEN_ERROR);
    if ((getLineBytes = getline((char ** restrict) & nString, (size_t * restrict) & nLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    n = (unsigned int) strtoul((const char * restrict) nString, (char ** restrict) NULL, 10);
    if (n == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOUL_SCOPE, STRTOUL_ERROR);

    * a = (float *) calloc(n, sizeof(* a));
    * b = (float *) calloc(n, sizeof(* b));
    if (!(* a) || !(* b)) raiseError(CALLOC_SCOPE, CALLOC_ERROR);
    for (int i = 0; i < n; i++) {
        if ((getLineBytes = getline((char ** restrict) & singleNumberString, (size_t * restrict) & singleNumberLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
        singleNumber = (float) strtof((const char *) singleNumberString, (char ** restrict) NULL);
        if ((singleNumber == 0.0F || singleNumber == HUGE_VALF) && (errno == ERANGE)) raiseError(STRTOF_SCOPE, STRTOF_ERROR);
        (* a)[i] = singleNumber;
        free(singleNumberString);
    }
    
    for (int i = 0; i < n; i++) {
        if ((getLineBytes = getline((char ** restrict) & singleNumberString, (size_t * restrict) & singleNumberLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
        singleNumber = (float) strtof((const char *) singleNumberString, (char ** restrict) NULL);
        if ((singleNumber == 0.0F || singleNumber == HUGE_VALF) && (errno == ERANGE)) raiseError(STRTOF_SCOPE, STRTOF_ERROR);
        (* b)[i] = singleNumber;
        free(singleNumberString);
    }

    if ((getLineBytes = getline((char ** restrict) & singleNumberString, (size_t * restrict) & singleNumberLength, (FILE * restrict) dataFilePointer)) == -1) raiseError(GETLINE_SCOPE, GETLINE_ERROR);
    * alpha = (float) strtof((const char *) singleNumberString, (char ** restrict) NULL);
    if ((* alpha == 0.0F || * alpha == HUGE_VALF) && (errno == ERANGE)) raiseError(STRTOF_SCOPE, STRTOF_ERROR);

    fclose(configurationFilePointer);
    fclose(dataFilePointer);
    free(masterProcessorID_string);
    free(dataFilePathString);
    free(nString);
    free(singleNumberString);
}