//
//  SaxpyLibrary.c
//  saxpy_mpi
//
//  Created by Denny Caruso on 28/05/22.
//

#include "UsageUtility.h"



/*
    Invoca la saxpy in parallelo oppure in sequenziale a seconda del valore assunto dalla variabile "saxpyMode".
    PARMS:
        - float * a: array a letto dal file di input.
        - float * b: array b letto dal file di input.
        - float ** c: puntatore ad array c in cui verrà conservato il vettore risultante finale.
        - float alpha: scalare dell'operazione saxpy.
        - unsigned int arraySize: dimensione di a, e quindi anche dimensione di b.
        - unsigned short int saxpyMode: modalità operazione saxpy, parallela o sequenziale.
        - int masterProcessorID: identificativo del processore master del communicator.
        - MPI_Comm commWorld: communicator usato di riferimento.
        - int processorID: identificativo del processore in esecuzione.
        - unsigned int nProcessor: numero di processori presenti all'interno del communicator.

*/
void saxpy (float * a, float * b, float ** c, float alpha, unsigned int arraySize, unsigned short int saxpyMode, int masterProcessorID, MPI_Comm commWorld, int processorID, unsigned int nProcessor) {
    switch (saxpyMode) {
        case SAXPY_SEQUENTIAL:
            saxpy_sequential(a, b, c, alpha, arraySize, commWorld);
            break;
        case SAXPY_PARALLEL:
            saxpy_parallel(a, b, c, alpha, arraySize, masterProcessorID, commWorld, processorID, nProcessor);
            break;
        default:
            raiseError(INVALID_SAXPY_MODE_SCOPE, INVALID_SAXPY_MODE_ERROR, commWorld, FALSE);
            break;
    }
}

/*
    Realizza l'operazione saxpy in parallelo: c = alpa * a + b.
    PARMS:
        - float * a: array a letto dal file di input.
        - float * b: array b letto dal file di input.
        - float ** c: puntatore ad array c in cui verrà conservato il vettore risultante finale.
        - float alpha: scalare dell'operazione saxpy.
        - unsigned int arraySize: dimensione di a, e quindi anche dimensione di b.
        - int masterProcessorID: identificativo del processore master del communicator.
        - MPI_Comm commWorld: communicator usato di riferimento.
        - int processorID: identificativo del processore in esecuzione.
        - unsigned int nProcessor: numero di processori presenti all'interno del communicator.
*/
void saxpy_parallel (float * a, float * b, float ** c, float alpha, unsigned int arraySize, int masterProcessorID, MPI_Comm commWorld, int processorID, unsigned int nProcessor) {
    int errorCode;
    int * recvcounts = NULL, * displacements = NULL, arraySizeLoc = 0;
    unsigned int remainder = 0;
    float * aLoc, * bLoc, * cLoc;
    aLoc = bLoc = cLoc = NULL;

    /*
        La variabile "errorCode" serve per mostrare gli eventuali codici di errore generati dalla chiamata delle routine 
        della libreria di MPI. 
        L'array "recvounts" servirà a tenere traccia delle dimensioni locali del problema per ogni processore mentre 
        "displacements" per tenere traccia degli spiazzamenti da considerare da qui a breve. Entrambi i vettori saranno infatti 
        utilizzati sia per ripartire gli elementi del vettore a e b, che per riunire gli elementi nel vettore c. 
        
        Di seguito è riportato un esempio con M = 23 e p = 5:
        i    count[i]   count[i]+1   displ[i]   displ[i]-displ[i-1]
        ------------------------------------------------------------
        0       5          6           0    
        1       5          6           5                 5
        2       5          6          10                 5
        3       4          5          15                 5
        4       4          5          19                 4

        La variabile "arraySizeLoc" è la dimensione della porzione di array a e b assegnata al processore. La variabile "remainder" 
        è il resto ottenuto dalla divisione fra la dimensione del problema ed il numero di processori.

        Poi si invia in broadcast dal processore master a tutti gli altri nel communicator lo scalare alpha coinvolto nell'operazione 
        saxpy.
    */
    if ((errorCode = MPI_Bcast(& alpha, 1, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_BCAST_SCOPE, errorCode, commWorld, FALSE); 
    if ((errorCode = MPI_Barrier(commWorld) != MPI_SUCCESS)) raiseError(MPI_BARRIER_SCOPE, errorCode, commWorld, FALSE);

    /*
        Ripartizione del numero di elementi totali fra i processori. Ogni processore con identificativo minore del resto, ottiene un 
        elemento in più da entrambi gli array da considerare per l'operazione saxpy. Successivamente, inizializzo i vettori "recvcounts" 
        e "displacements".
    */
    arraySizeLoc = arraySize / nProcessor;
    remainder = arraySize % nProcessor;
    if (remainder > 0) {
        if (processorID < remainder) {
            arraySizeLoc += 1;
        }
    }

    if (processorID == masterProcessorID) {
        recvcounts = createIntArray(nProcessor, commWorld);
        displacements = createIntArray(nProcessor, commWorld);
        displacements[0] = 0;
    }

    /*
        Si ricava l'array "recvcounts" tramite "MPI_Gather(...) con le varie dimensioni locali "arratSizeLoc". Si ricava poi l'array 
        degli spiazzamenti "displacements" da considerare. Poi si alloca la memoria per gli array locali "aLoc", "bLoc" e "cLoc".
    */
    if ((errorCode = MPI_Gather(& arraySizeLoc, 1, MPI_INT, recvcounts, 1, MPI_INT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_GATHER_SCOPE, errorCode, commWorld, FALSE);
    if (processorID == masterProcessorID) for (int i = 1; i < nProcessor; i++) displacements[i] = displacements[i - 1] + recvcounts[i - 1];

    aLoc = createFloatArray(arraySizeLoc, commWorld);
    bLoc = createFloatArray(arraySizeLoc, commWorld);
    cLoc = createFloatArray(arraySizeLoc, commWorld);

    /*
        Si effettua la suddivisione degli elementi di a e di b in base a dimensioni locali potenzialmente differenti senza difficoltà 
        grazie agli array "recvcounts" e "displacements". Si effettua l'operazione saxpy in locale su tali vettori, che permette di 
        ottenere il risultato locale in "cLoc". A questo punto si effettua la "ricostruzione" del vettore finale "c" grazie ai due 
        array precedentemente costruiti "recvcounts" e "displacements". Si precisa che non si usa la "MPI_Scatter(...)" e la 
        "MPI_Gather(...)", ma la  "MPI_Scatterv(...)" e la "MPI_Gatherv(...)", in quanto è necessario prevedere anche una suddivisibilità 
        non esatta del numero della dimensione del problema per il numero di processori presenti nel communicator.
        Infine, si rilascia la memoria allocata e non più utilizzata.
    */
    if ((errorCode = MPI_Scatterv(a, recvcounts, displacements, MPI_FLOAT, aLoc, arraySizeLoc, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_SCATTERV_SCOPE, errorCode, commWorld, FALSE); 
    if ((errorCode = MPI_Scatterv(b, recvcounts, displacements, MPI_FLOAT, bLoc, arraySizeLoc, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_SCATTERV_SCOPE, errorCode, commWorld, FALSE); 
    saxpy_sequential(aLoc, bLoc, & cLoc, alpha, arraySizeLoc, commWorld);
    if ((errorCode = MPI_Gatherv(cLoc, arraySizeLoc, MPI_FLOAT, * c, recvcounts, displacements, MPI_FLOAT, masterProcessorID, commWorld)) != MPI_SUCCESS) raiseError(MPI_GATHERV_SCOPE, errorCode, commWorld, FALSE);
    
    releaseMemory(aLoc, bLoc, cLoc, (void *) 0);
    if (processorID == masterProcessorID) releaseMemory(recvcounts, displacements, (void *) 0);
}

/*
    Realizza l'operazione saxpy in sequenziale: c = alpa * a + b.
    PARMS:
        - float * a: array a letto dal file di input.
        - float * b: array b letto dal file di input.
        - float ** c: puntatore ad array c in cui verrà conservato il vettore risultante finale.
        - float alpha: scalare dell'operazione saxpy.
        - unsigned int arraySize: dimensione di a, e quindi anche dimensione di b.
        - MPI_Comm commWorld: communicator usato di riferimento.
*/
void saxpy_sequential (float * a, float * b, float ** c, float alpha, unsigned int arraySize, MPI_Comm commWorld) {
    for (int i = 0; i < arraySize; i++) * ((* c) + i) = (alpha * (* (a + i))) + (* (b + i));
}