//
//  saxpy.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 21/05/22.
//

/*
    Somma tra un vettore "a" moltiplicato per uno scalare "alpha" e un altro vettore "b" 
    in parallelo in ambiente MPI-Docker (c = alpha * a + b).
*/

#include "UsageUtility.h"



int main (int argc, char ** argv) {
    /*
        Il numero di parametri atteso è 3: nome eseguibile, path relativo del file di configurazione e ID del processore master.
        La variabile "outputFilePath" è il path relativo del file di output dove salvare il risultato calcolato.
        La variabile "errorCode" serve per mostrare gli eventuali codici di errore generati dalla chiamata delle routine 
        della libreria di MPI.
        La variabile "totalTime" contiene il tempo totale per ogni processore impiegato per effettuare l'operazione di calcolo e 
        le altre operazioni annesse. Mentre la variabile "maxTime" conterrà solo il tempo massimo fra tutti i "tempi totali impiegati" dai 
        processori.
    */
    const int expectedArgc = 3;
    const char * expectedUsageMessage = "<configuration filepath> <master processor ID>";
    char * outputFilePath = NULL;

    int masterProcessorID, processorID, errorCode;
    unsigned int arraySize = 0,  nProcessor = 0;
    unsigned short int saxpyChosenMode;

    float * a, * b, * c, alpha;
    double startTime, endTime, totalTime, maxTime;
    a = b = c = NULL;
    MPI_Comm myCommWorld = MPI_COMM_WORLD;

    /* 
        Inizializzazione dell'ambiente MPI, si ricava poi l'identificativo del processore all'interno del communicator e il numero di 
        processori presenti in esso. Si verificano i parametri passati da linea di comando e tra questi si converte il 
        "masterProcessorID" in formato numerico intero, con verifica degli errori annessa. Infine, si imposta l'ambiente di 
        calcolo con "setEnvironment(...)": allocazione memoria, lettura impostazioni, dati, etc.
    */
    if ((errorCode = MPI_Init(& argc, & argv)) != MPI_SUCCESS) raiseError(MPI_INIT_SCOPE, errorCode, myCommWorld, FALSE);
    if ((errorCode = MPI_Comm_rank(myCommWorld, & processorID)) != MPI_SUCCESS) raiseError(MPI_COMM_RANK_SCOPE, errorCode, myCommWorld, FALSE);
    if ((errorCode = MPI_Comm_size(myCommWorld, & nProcessor)) != MPI_SUCCESS) raiseError(MPI_COMM_SIZE_SCOPE, errorCode, myCommWorld, FALSE);
    
    checkUsage(argc, (const char **) argv, expectedArgc, expectedUsageMessage, myCommWorld);
    masterProcessorID = (int) strtol((const char * restrict) argv[2], (char ** restrict) NULL, 10);
    if (masterProcessorID == 0 && (errno == EINVAL || errno == ERANGE)) raiseError(STRTOL_SCOPE, STRTOL_ERROR, myCommWorld, FALSE);
    if (processorID == masterProcessorID) setEnvironment(& a, & b, & alpha, & c, & arraySize, argv[1], & outputFilePath, & saxpyChosenMode, myCommWorld);
    
    /*
        Si stabilisce una barriera di sincronizzazione dopo aver impostato l'ambiente e si prende il tempo di inizio. Poi si invia in 
        broadcast dal processore master a tutti gli altri nel communicator la modalità di operazione saxpy scelta e la dimensione dei due 
        array ricavati dal file di dati in input. Fatto ciò, si invoca l'operazione di "saxpy(...)".
    */
    if ((errorCode = MPI_Barrier(myCommWorld) != MPI_SUCCESS)) raiseError(MPI_BARRIER_SCOPE, errorCode, myCommWorld, FALSE);
    startTime = MPI_Wtime();
    if ((errorCode = MPI_Bcast(& saxpyChosenMode, 1, MPI_UNSIGNED_SHORT, masterProcessorID, myCommWorld)) != MPI_SUCCESS) raiseError(MPI_BCAST_SCOPE, errorCode, myCommWorld, FALSE); 
    if ((errorCode = MPI_Bcast(& arraySize, 1, MPI_UNSIGNED, masterProcessorID, myCommWorld)) != MPI_SUCCESS) raiseError(MPI_BCAST_SCOPE, errorCode, myCommWorld, FALSE); 
    
    saxpy(a, b, & c, alpha, arraySize, saxpyChosenMode, masterProcessorID, myCommWorld, processorID, nProcessor);
    
    /*
        Si stabilisce una barriera di sincronizzazione così da prendere correttamente il tempo di fine. Si calcola il tempo totale 
        per ogni processore e si ricava quello massimo. Infine, il processore "master" stampa il messaggio di output per l'utente, 
        salva il risultato nel file di outptut e rilascia la memoria allocata. Infine, si termina l'ambiente MPI e il programma termina.
    */
    if ((errorCode = MPI_Barrier(myCommWorld) != MPI_SUCCESS)) raiseError(MPI_BARRIER_SCOPE, errorCode, myCommWorld, FALSE);
    endTime = MPI_Wtime();
    totalTime = endTime - startTime;
    MPI_Reduce(& totalTime, & maxTime, 1, MPI_DOUBLE, MPI_MAX, masterProcessorID, myCommWorld);
    if (processorID == masterProcessorID) {
        if (fprintf(stdout, (const char * restrict) OUTPUT_USER_MESSAGE, outputFilePath, arraySize, alpha, maxTime) < 0) {
            raiseError(FPRINTF_SCOPE, FPRINTF_ERROR, myCommWorld, FALSE);
        }

        saveResult(c, arraySize, outputFilePath, myCommWorld);
        releaseMemory(a, b, c, outputFilePath, (void *) 0);
    }
    
    if ((errorCode = MPI_Finalize() != MPI_SUCCESS)) raiseError(MPI_FINALIZE_SCOPE, errorCode, myCommWorld, FALSE);
    exit(0);
}