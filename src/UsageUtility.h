//
//  UsageUtility.h
//  mpi_project
//
//  Created by Denny Caruso on 21/05/22.
//

#ifndef UsageUtility_h
#define UsageUtility_h

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



// Error code and scope message
static const int CHECK_USAGE_ERROR = 1;
static const char * CHECK_USAGE_SCOPE = "check usage";

static const int FPRINTF_ERROR = 2;
static const char * FPRINTF_SCOPE = "generic fprintf";

static const int FOPEN_ERROR = 3;
static const char * FOPEN_SCOPE = "generic fopen";

static const int GETLINE_ERROR = 4;
static const char * GETLINE_SCOPE = "generic getline";

static const int CALLOC_ERROR = 5;
static const char * CALLOC_SCOPE = "generic calloc";

static const int CONFIGURATION_FILE_OPEN_ERROR = 6;
static const char * CONFIGURATION_FILE_OPEN_SCOPE = "setEnvironment/configurationFile opening";

static const int DATA_FILE_OPEN_ERROR = 7;
static const char * DATA_FILE_OPEN_SCOPE = "setEnvironment/dataFile opening";

static const int STRTOUL_ERROR = 8;
static const char * STRTOUL_SCOPE = "strtoul";

static const int STRTOF_ERROR = 9;
static const char * STRTOF_SCOPE = "strtof";



enum boolean { FALSE, TRUE };



void        checkUsage          (int argc,                  const char * argv[],    int expected_argc, const char * expectedUsageMessage                            );
void        raiseError          (const char * errorScope,   int exitCode                                                                                            );
void        setEnvironment      (float ** a,                float ** b,             float * alpha,     const char * configurationFilePath, int * masterProcessorID  );

#endif /* UsageUtility_h */
