/* Assignment 2
* Pietro Baggetta
* 1092160
* Citations in code:
    * Lecture Code
    * Textbook Code
    * Programiz (heap insertion implementation)
    * Algorithm Tutor (heap deleting implementation)
*/
#include "myHeader.h"

/* Find the next number in the line */
int getNextNum (char *data) {
    int num = 0;

    /* Find the number of threads by looking after space */
    char *token = strrchr (data, ' ');

    if (token == NULL) {
        token = data;
    } else {
        token++;
    }

    num = atoi (token);

    /* Return number of threads */
    return num;
}

/* Get the arrival time from the line */
int getArrivalTime (char *data) {
    int num = 0;

    char *token = &data[2];

    num = atoi (token);

    /* Return the arrival time */
    return num;
}

/* Get / calculate the burst time for the thread */
void getBurstTime (Thread *currThread, int notDone) {

    char *line;
    int total =  0, burstNum = 0, ioNum = 0;

    if (notDone == 1){
        /* For RR burst times */
        int current = currThread -> currentBurst;
        current = current - 2;
        line = currThread -> bursts[current];
        total = current;
    } else {
        /* For FCFS burst times */
        line = currThread -> bursts[currThread -> currentBurst - 1];
        total = currThread -> currentBurst;

        /* Get the IO bursts and bursts */
        char *token = strtok (line, " ");
        token = strtok (NULL, " ");
        burstNum = atoi (token);

        token = strtok (NULL, " ");
        if (token != NULL) {
            ioNum = atoi (token);
        }
        
        currThread -> cpuBurst = burstNum;
    }

    /* Calculate the burst */
    int burst = currThread -> burstTime;
  
    currThread -> totalCpuBurst = currThread -> totalCpuBurst + currThread -> cpuBurst;

    if (total < currThread -> numBursts) {
        if(notDone == 1) {
            burst = currThread -> cpuBurst;
        } else {
            burst = currThread -> cpuBurst + ioNum;
            currThread -> ioBurst = ioNum + currThread -> ioBurst;
        }
    } else {
        burst = currThread->cpuBurst;
    }
    /* Set the burst time for the struct/thread */
    currThread->burstTime = burst;
}

/* Get the time the thread/process enteres the cpu */
int getCpuEntryTime (Thread *currThread, int cpuBurst, int pNum, int tNum, int *switchProcess, int *switchThread, int cpuEnter) {
    int entry = 0;

    if (currThread -> processNum == pNum && currThread -> threadNum == tNum) {
        entry = cpuBurst + cpuEnter + currThread -> ioBurst;
    } else if (currThread -> processNum == pNum && currThread -> threadNum != tNum) {
        entry = cpuBurst + cpuEnter + *switchThread;
    } else {
        entry = cpuBurst + cpuEnter + *switchProcess;
    }

    currThread -> enterCpu = entry;

    return entry;
}

/* Calculate the ready time for the current thread */
void getReadyTime (Thread *currThread) {
    int ready = currThread -> enterCpu + currThread -> burstTime;

    currThread -> readyQ = ready;
}

/* Calculate the turnaround time for the current thread */
void turnaroundTime (Thread *currThread) {
    currThread -> turnaround = currThread -> finishTime - currThread -> arrivalTime;
}

/* Break apart the input and store it into the struct */
void breakInput (char data[100][100], int lineCount, int *numProcess, int *switchThread, int *switchProcess, Thread **heap, int *heapCounter) {

    int numP = 0, switchT = 0, switchP = 0;
    int i, j, k;
    int numThreads = 0, burstCounter = 0, arrivalTime = 0, processNum = 0, threadNum = 0, bursts = 0;

    char *token = strtok (data[0], " ");
    /* - Get number of processes,
       - Time to switch to a new thread in same process and
       - Time to switch to a new thread in different process */

    /* Number of processes */
    numP = atoi (token);
    token = strtok (NULL, " ");

    /* Time to swtich to new thread in same process */
    switchT = atoi (token);
    token = strtok (NULL, " ");

    /* Time to switch to new thread in different process */
    switchP = atoi (token);
    token = strtok (NULL, " ");

    *numProcess = numP;
    *switchThread = switchT;
    *switchProcess = switchP;

    i = 1;
    while (i < lineCount) {

        /* Only execute when new process is found */
        if (strlen (data[i]) == 3) {
            /* increase process number counter */
            processNum++;
            /* get number of threads in current process */
            numThreads = getNextNum (data[i]);

            i++;
            k = 0;
            threadNum = 0;

            /* loop through each thread in the process individually */
            while (k < numThreads) {
                threadNum++;
                /* get the number of bursts for the thread */
                burstCounter = getNextNum (data[i]);
                /* get the arrival time of the thread */
                arrivalTime = getArrivalTime (data[i]);

                Thread *p = malloc (sizeof (Thread));

                /* set each variable in the struct for the current thread */
                p -> processNum = processNum;
                p -> threadNum = threadNum;
                p -> arrivalTime = arrivalTime;
                p -> numBursts = burstCounter;
                p -> readyQ = p -> arrivalTime;
                p -> currentBurst = 0;
                p -> cpuBurst = 0;
                p -> totalCpuBurst = 0;
                p -> ioBurst = 0;
                p -> burstTime = 0;
                p -> startTime = 0;
                p -> notDone = 0;
                p -> finishTime = 0;
                p -> enterCpu = p -> arrivalTime;
                p -> entered = 0;
                p -> turnaround = 0;
                
                i++;

                j = 0;
                bursts = 0;

                /* Get the thread bursts from the data and set into struct variable */
                while (j < burstCounter) {
                    strcpy (p -> bursts[bursts], data[i]);

                    bursts++;
                    i++;
                    j++;
                }

                insertToHeap (p, heap, heapCounter);

                k++;
            }
        }
    }
}

/* Print the entire heap (FOR TESTING) */
void printHeap (Thread **heap, int *heapCounter) {

    for (int i = 0; i < *heapCounter; i++) {
        printf("%d %d %d\n", heap[i]->threadNum, heap[i]->readyQ, heap[i]->numBursts);
        for (int j = 0; j <= heap[i]->numBursts; j++)
        {
            printf("%s\n", heap[i]->bursts[j]);
        }
    }
}

/* Print the detailed information for the -d flag */
void printDetailed (Thread **heap, int *heapCounter) {
    
    for (int i = 0; i < *heapCounter; i++) {
        printf ("Thread %d of Process %d:\n", heap[i] -> threadNum, heap[i] -> processNum);
        printf ("    arrival time: %d\n", heap[i] -> arrivalTime);
        printf ("    service time: %d units, I/O time: %d units, turnaround time: %d units, finish time: %d units\n", heap[i] -> totalCpuBurst, heap[i] -> ioBurst, heap[i] -> turnaround, heap[i] -> finishTime);
    }
}

/* Print the verbose information for the -v flag */
void printVerbose (VerboseInfo **heap, int *verboseCounter) {

    while (*verboseCounter > 0) {
        VerboseInfo *v = heap[0];

        if (v -> newToReady == 1) {
            printf ("At time %d: Thread %d of Process %d goes from new to ready\n", v -> time, v -> thread, v -> process);
        } else if (v -> readyToRun == 1) {
            printf ("At time %d: Thread %d of Process %d goes from ready to running\n", v -> time, v -> thread, v -> process);
        } else if (v -> runToTerm == 1) {
            printf ("At time %d: Thread %d of Process %d goes from running to terminated\n", v -> time, v -> thread, v -> process);
        } else if (v -> runToBlock == 1) {
            printf ("At time %d: Thread %d of Process %d goes from running to blocked\n", v -> time, v -> thread, v -> process);
        } else if (v -> blockToReady == 1) {
            printf ("At time %d: Thread %d of Process %d goes from blocked to ready\n", v -> time, v -> thread, v -> process);
        }

        verboseRemoveRoot (heap, verboseCounter, 0);
    }
}
