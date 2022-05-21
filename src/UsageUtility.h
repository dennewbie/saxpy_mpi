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



// Error code and scope message
const int CHECK_USAGE_ERROR = 1;
const char * CHECK_USAGE_SCOPE = "usage";

const int FRPINTF_ERROR = 2;
const char * FPRINTF_SCOPE = "fprintf";

const int FOPEN_ERROR = 3;
const char * FOPEN_SCOPE = "fopen";

const int GETLINE_ERROR = 4;
const char * GETLINE_SCOPE = "getline";

const int CALLOC_ERROR = 6;
const char * CALLOC_SCOPE = "calloc";



enum boolean                    { FALSE,                TRUE                                                                        };

void        checkUsage          (int argc,              const char * argv[],    int expected_argc, const char * expectedUsageMessage);
void        raiseError          (char * errorScope,     int exitCode                                                                );

#endif /* UsageUtility_h */
