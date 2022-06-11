#ifndef _MYHEADER_H
#define _MYHEADER_H

/* header files used in program */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* struct to hold all the threads and process information */
typedef struct {
    int numThreads;

    int processNum;
    int threadNum;
    int arrivalTime;
    int numBursts;

    int readyQ;

    int currentBurst;
    int cpuBurst;
    int totalCpuBurst;
    int ioBurst;
    char bursts[100][100];

    int burstTime;
    int startTime;
    int notDone;
    int finishTime;
    int enterCpu;
    int entered;
    
    int turnaround;

} Thread;

/* struct to hold all the verbose information */
typedef struct {
    int process;
    int thread;

    int time;
    int newToReady;
    int readyToRun;
    int runToTerm;
    int runToBlock;
    int blockToReady;

} VerboseInfo;

/* function declaration for heaper_functions.c */
int getNextNum (char *data);
int getArrivalTime (char *data);
void getBurstTime (Thread * currThread, int isntDone);
int getCpuEntryTime (Thread *currThread, int cpuBurst, int pNum, int tNum, int *switchProcess, int *switchThread, int cpuEnter);
void getReadyTime (Thread *currThread);
void turnaroundTime (Thread *currThread);
void breakInput (char data[100][100], int lineCount, int *numProcess, int *switchThread, int *switchProcess, Thread **heap, int *heapCounter);
void printHeap (Thread **heap, int *heapCounter);
void printDetailed (Thread **heap, int *heapCounter);
void printVerbose (VerboseInfo **heap, int *verboseCounter);

/* function declaration for heap_functions.c */
int parentIndex (int index);
int leftIndex (int index);
int rightIndex (int index);
void heapify (Thread **heap, int *heapCounter, int index);
void verboseHeapify (VerboseInfo **heap, int *heapCounter, int index);
void insertToHeap (Thread *currThread, Thread **heap, int *heapCounter);
void insertHeapVerbose (VerboseInfo *currThread, VerboseInfo **heap, int *heapCounter);
void heapFinished (Thread *thread, Thread **heap, int *heapCounter);
void removeRoot (Thread **heap, int *heapCounter, int num);
void verboseRemoveRoot (VerboseInfo **heap, int *heapCounter, int num);

#endif
