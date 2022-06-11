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

int main (int argc, char *argv[]) {

    int len = 0, FCFS = 0, RR = 0, verbose = 0, detailInfo = 0, quantumNumber;
    int lineCount = 0, numProcess = 0, switchThread = 0, switchProcess = 0;
    int heapCounter = 0;
    char data[100][100];

    for (int i = 0; i < argc; i++) {
        /* Checks for Round Robin mode */
        if (!strcmp (argv[i], "-r")) {
            RR = 1;
            FCFS = 0;
            quantumNumber = atoi (argv[i + 1]);
        }
        /* Checks for detail flag */
        if (!strcmp (argv[i], "-d")) {
            detailInfo = 1;
        }
        /* Checks for verbose flag */
        if (!strcmp (argv[i], "-v")) {
            verbose = 1;
        }
    }
    /* If RR is not the mode, default to FCFS */
    if (RR == 0) {
        FCFS = 1;
    }

    /** First Come First Serve **/
    if (FCFS == 1) {
        printf ("\n*************** FCFS Scheduling ***************\n\n");
        len = 0;
        /* Reads in the file that is redirected from stdin */
        while (fgets (data[lineCount], sizeof (data), stdin)) {
            len = strlen (data[lineCount]);
            
            /* - Replaces new line character with end of string.
               - This is to ensure when working with the numbers/threads
                no overhang/misread is possible */
            if (data[lineCount][len - 1] == '\n') {
                data[lineCount][len - 1] = '\0';
            }
            lineCount++;
        }

        /* store all the thread in this array */
        Thread **heap;
        heap = malloc (sizeof (Thread *) * 1024);
        heapCounter = 0;

        /* read in the file and break the input into the struct */
        breakInput (data, lineCount, &numProcess, &switchThread, &switchProcess, heap, &heapCounter);
        /* printHeap (heap, &heapCounter); */ /* USED FOR TESTING */
    
        int k = 0, readyQ = 0, cpuEnter = 0, cpuBurst = 0, pNum = 0, tNum = 0, totalTime = 0, first = 0, last = 0, totalCpuBurst = 0;
        int finishCounter = 0, verboseCounter = 0;
        double turnaround = 0.0;
        float cpuUtilization = 0.0;
        /* create struct array for finished result */
        Thread **finished = malloc (sizeof (Thread *) * 1024);
        /* create struct array for verbose mode */
        VerboseInfo **v = malloc (sizeof (VerboseInfo *) * 5000);

        /* Loop through all threads until all executed */
        while (heapCounter > 0) {
            Thread *currThread = heap[0];

            currThread -> currentBurst = currThread -> currentBurst + 1;
            readyQ = currThread -> readyQ;

            if ((verbose == 1) && (currThread -> currentBurst == 1)) {
                /* verbose mode: new to running state */
                VerboseInfo *newRunning = malloc (sizeof (VerboseInfo));
                newRunning -> process = currThread -> processNum;
                newRunning -> thread = currThread -> threadNum;
                newRunning -> time = readyQ;
                newRunning -> newToReady = 1;
                newRunning -> readyToRun = 0;
                newRunning -> runToTerm = 0;
                newRunning -> runToBlock = 0;
                newRunning -> blockToReady = 0;
                insertHeapVerbose (newRunning, v, &verboseCounter);
            } else if (verbose == 1) {
                /* verbose mode: blocked to ready state */
                VerboseInfo *blockReady = malloc (sizeof (VerboseInfo));
                blockReady -> process = currThread -> processNum;
                blockReady -> thread = currThread -> threadNum;
                blockReady -> time = readyQ;
                blockReady -> newToReady = 0;
                blockReady -> readyToRun = 0;
                blockReady -> runToTerm = 0;
                blockReady -> runToBlock = 0;
                blockReady -> blockToReady = 1;
                insertHeapVerbose (blockReady, v, &verboseCounter);
            }

            /* Get the burst time */
            getBurstTime (currThread, 0);
            totalCpuBurst += currThread -> cpuBurst;

            /* Cpu entry time */
            if (k == 0) {
                cpuEnter = 0;
                currThread -> startTime = cpuEnter;
            } else {
                cpuEnter = getCpuEntryTime (currThread, cpuBurst, pNum, tNum, &switchProcess, &switchThread, cpuEnter);
            }
            /* Set the cpu enter time */
            currThread -> enterCpu = cpuEnter;

            if (verbose == 1) {
                /* verbose mode: ready to running state */
                VerboseInfo *readyRun = malloc (sizeof (VerboseInfo));
                readyRun -> process = currThread -> processNum;
                readyRun -> thread = currThread -> threadNum;
                readyRun -> time = cpuEnter;
                readyRun -> newToReady = 0;
                readyRun -> readyToRun = 1;
                readyRun -> runToTerm = 0;
                readyRun -> runToBlock = 0;
                readyRun -> blockToReady = 0;
                insertHeapVerbose (readyRun, v, &verboseCounter);
            }

            /* Get and set the ready time */
            getReadyTime (currThread);
            readyQ = currThread -> readyQ;

            if (currThread -> currentBurst == 1) {
                currThread -> startTime = cpuEnter;
            }

            /* Set temporary variables to process number, thread number and cpu bursts */
            pNum = currThread -> processNum;
            tNum = currThread -> threadNum;
            cpuBurst = currThread -> cpuBurst;

            /* All bursts have executed */
            if (currThread -> currentBurst ==  currThread -> numBursts) {
                currThread -> finishTime = readyQ;
                totalTime = currThread -> finishTime;

                if (verbose == 1) {
                    /* verbose mode: Running to terminated state */
                    VerboseInfo *runTerminated = malloc (sizeof (VerboseInfo));
                    runTerminated -> process = currThread -> processNum;
                    runTerminated -> thread = currThread -> threadNum;
                    runTerminated -> time = currThread -> finishTime;
                    runTerminated -> newToReady = 0;
                    runTerminated -> readyToRun = 0;
                    runTerminated -> runToTerm = 1;
                    runTerminated -> runToBlock = 0;
                    runTerminated -> blockToReady = 0;
                    insertHeapVerbose (runTerminated, v, &verboseCounter);
                }

                /* Calculate turnaround time */
                turnaroundTime (currThread);
                if (currThread -> processNum == 1) {
                    first = currThread -> finishTime - currThread -> arrivalTime;
                }

                if (currThread -> processNum == numProcess) {
                    last = currThread -> finishTime - currThread -> arrivalTime;
                }

                heapFinished (currThread, finished, &finishCounter);
                removeRoot (heap, &heapCounter, 0);

            } else {
                /* Not all bursts have executed */
                if (verbose == 1) {
                    /* verbose mode: running to bloacked state */
                    VerboseInfo *runBlocked = malloc (sizeof (VerboseInfo));
                    runBlocked -> process = currThread -> processNum;
                    runBlocked -> thread = currThread -> threadNum;
                    runBlocked -> time = currThread -> finishTime;
                    runBlocked -> newToReady = 0;
                    runBlocked -> readyToRun = 0;
                    runBlocked -> runToTerm = 0;
                    runBlocked -> runToBlock = 1;
                    runBlocked -> blockToReady = 0;
                    insertHeapVerbose (runBlocked, v, &verboseCounter);
                }

                removeRoot (heap, &heapCounter, 0);
                insertToHeap (currThread, heap, &heapCounter);
            }
            k++; 
        }
        turnaround = (last + first) / 2.0;
        cpuUtilization = (float)totalCpuBurst / (float)totalTime * 100;

        if (argc < 2) {
            /* No flags entered */
            printf("Total Time Required is %d units\n", totalTime);
            printf("Average Turnaround Time is %g units\n", turnaround);
            printf("CPU Utilization is %g %%\n", cpuUtilization);
            return 0;
        }

        if (detailInfo == 1 && verbose == 0) {
            /* Detailed flag only */
            printf("Total Time Required is %d units\n", totalTime);
            printf("Average Turnaround Time is %g units\n", turnaround);
            printf("CPU Utilization is %g %%\n\n", cpuUtilization);
            printDetailed (finished, &finishCounter);
            return 0;
        }

        if (detailInfo == 0 && verbose == 1) {
            /* Verbose flag only */
            printf("Total Time Required is %d units\n", totalTime);
            printf("Average Turnaround Time is %g units\n", turnaround);
            printf("CPU Utilization is %g %%\n\n", cpuUtilization);
            printVerbose (v, &verboseCounter);
        }

        if (detailInfo == 1 && verbose == 1) {
            /* Detailed and verbose flag */
            printf("Total Time Required is %d units\n", totalTime);
            printf("Average Turnaround Time is %g units\n", turnaround);
            printf("CPU Utilization is %g %%\n\n", cpuUtilization);
            printDetailed (finished, &finishCounter);
            printf ("\n");
            printVerbose (v, &verboseCounter);
        }

        free (heap);
        free (finished);
        free (v);
    }

    /** Round Robin **/
    if (RR == 1) {
        printf ("\n********** Round Robin Scheduling (quantum = %d time units) **********\n\n", quantumNumber);
        len = 0;
        /* Reads in the file that is redirected from stdin */
        while (fgets (data[lineCount], sizeof (data), stdin)) {
            len = strlen (data[lineCount]);
            
            /* - Replaces new line character with end of string.
               - This is to ensure when working with the numbers/threads
                no overhang/misread is possible */
            if (data[lineCount][len - 1] == '\n') {
                data[lineCount][len - 1] = '\0';
            }
            lineCount++;
        }

        /* store all the thread in this array */
        Thread **heap;
        heap = malloc (sizeof (Thread *) * 1024);
        heapCounter = 0;

        /* read in the file and break the input into the struct */
        breakInput (data, lineCount, &numProcess, &switchThread, &switchProcess, heap, &heapCounter);

        int k = 0, readyQ = 0, cpuEnter = 0, cpuBurst = 0, pNum = 0, tNum = 0, totalTime = 0, first = 0, last = 0, totalCpuBurst = 0;
        int finishCounter = 0, verboseCounter = 0, counter = 0, waitTime = 0;
        double turnaround = 0.0;
        float cpuUtilization = 0.0;
        /* create struct array for finished result */
        Thread **finished = malloc (sizeof (Thread *) * 1024);
        /* create struct array for verbose mode */
        VerboseInfo **v = malloc (sizeof (VerboseInfo *) * 5000);

        /* Loop through all threads until all executed */
        while (heapCounter > 0) {
            Thread *currThread = heap[0];

            /* Calculate current burst */
            if (currThread -> currentBurst < currThread -> numBursts) {
                currThread -> currentBurst = currThread -> currentBurst + 1;
            }
            readyQ = currThread -> readyQ;

            if ((verbose == 1) && (currThread -> currentBurst == 1)) {
                /* verbose mode: new to running state */
                VerboseInfo *newRunning = malloc (sizeof (VerboseInfo));
                newRunning -> process = currThread -> processNum;
                newRunning -> thread = currThread -> threadNum;
                newRunning -> time = readyQ;
                newRunning -> newToReady = 1;
                newRunning -> readyToRun = 0;
                newRunning -> runToTerm = 0;
                newRunning -> runToBlock = 0;
                newRunning -> blockToReady = 0;
                insertHeapVerbose (newRunning, v, &verboseCounter);
            } else if (verbose == 1) {
                /* verbose mode: blocked to ready state */
                VerboseInfo *blockReady = malloc (sizeof (VerboseInfo));
                blockReady -> process = currThread -> processNum;
                blockReady -> thread = currThread -> threadNum;
                blockReady -> time = readyQ;
                blockReady -> newToReady = 0;
                blockReady -> readyToRun = 0;
                blockReady -> runToTerm = 0;
                blockReady -> runToBlock = 0;
                blockReady -> blockToReady = 1;
                insertHeapVerbose (blockReady, v, &verboseCounter);
            }

            /* Get the burst time */
            getBurstTime (currThread, currThread -> notDone);

            /* Calculate how much of the burst executes */
            if (currThread -> cpuBurst <= quantumNumber) {
                currThread -> cpuBurst = currThread -> cpuBurst - quantumNumber;
                currThread -> burstTime = currThread -> burstTime - currThread -> ioBurst - quantumNumber;
            } else {
                currThread -> notDone = 0;
            }

            totalCpuBurst += currThread -> cpuBurst;

            /* Cpu entry time */
            if (k == 0) {
                cpuEnter = 0;
                currThread -> startTime = cpuEnter;
            } else {
                cpuEnter = getCpuEntryTime (currThread, cpuBurst, pNum, tNum, &switchProcess, &switchThread, cpuEnter);
            }

            if (verbose == 1) {
                /* verbose mode: ready to running state */
                VerboseInfo *readyRun = malloc (sizeof (VerboseInfo));
                readyRun -> process = currThread -> processNum;
                readyRun -> thread = currThread -> threadNum;
                readyRun -> time = cpuEnter;
                readyRun -> newToReady = 0;
                readyRun -> readyToRun = 1;
                readyRun -> runToTerm = 0;
                readyRun -> runToBlock = 0;
                readyRun -> blockToReady = 0;
                insertHeapVerbose (readyRun, v, &verboseCounter);
            }

            /* Get and set the ready time */
            getReadyTime (currThread);
            readyQ = currThread -> readyQ;
            currThread -> enterCpu = cpuEnter;

            if (currThread -> currentBurst == 1) {
                currThread -> startTime = cpuEnter;
            }

            /* Set temporary variables to process number, thread number and cpu bursts */
            pNum = currThread -> processNum;
            tNum = currThread -> threadNum;
            cpuBurst = currThread -> cpuBurst;


            /* If the process is finished */
            if ((currThread -> currentBurst == currThread -> numBursts) && (currThread -> notDone == 0)) {
                currThread -> finishTime = readyQ;
                totalTime = currThread -> finishTime;

                if (verbose == 1) {
                    /* verbose mode: Running to terminated state */
                    VerboseInfo *runTerminated = malloc (sizeof (VerboseInfo));
                    runTerminated -> process = currThread -> processNum;
                    runTerminated -> thread = currThread -> threadNum;
                    runTerminated -> time = currThread -> finishTime;
                    runTerminated -> newToReady = 0;
                    runTerminated -> readyToRun = 0;
                    runTerminated -> runToTerm = 1;
                    runTerminated -> runToBlock = 0;
                    runTerminated -> blockToReady = 0;
                    insertHeapVerbose (runTerminated, v, &verboseCounter);
                }

                /* Calculate turnaround time */
                turnaroundTime (currThread);
                if (currThread -> processNum == 1) {
                    first = currThread -> finishTime - currThread -> arrivalTime;
                }

                if (currThread -> processNum == numProcess) {
                    last = currThread -> finishTime - currThread -> arrivalTime;
                }

                heapFinished (currThread, finished, &finishCounter);
                removeRoot (heap, &heapCounter, 0);
            } else if ((currThread -> currentBurst ==  currThread -> numBursts) && (currThread -> notDone == 1)) {
                /* If thread is on the last burst but not yet complete */
                if (verbose == 1) {
                    /* verbose mode: running to bloacked state */
                    VerboseInfo *runBlocked = malloc (sizeof (VerboseInfo));
                    runBlocked -> process = currThread -> processNum;
                    runBlocked -> thread = currThread -> threadNum;
                    runBlocked -> time = currThread -> finishTime;
                    runBlocked -> newToReady = 0;
                    runBlocked -> readyToRun = 0;
                    runBlocked -> runToTerm = 0;
                    runBlocked -> runToBlock = 1;
                    runBlocked -> blockToReady = 0;
                    insertHeapVerbose (runBlocked, v, &verboseCounter);
                }

                removeRoot (heap, &heapCounter, 0);
                insertToHeap (currThread, heap, &heapCounter);
            } else {
                /* Thread has more bursts to finish */
                if (verbose == 1) {
                    /* verbose mode: running to bloacked state */
                    VerboseInfo *runBlocked = malloc (sizeof (VerboseInfo));
                    runBlocked -> process = currThread -> processNum;
                    runBlocked -> thread = currThread -> threadNum;
                    runBlocked -> time = currThread -> finishTime;
                    runBlocked -> newToReady = 0;
                    runBlocked -> readyToRun = 0;
                    runBlocked -> runToTerm = 0;
                    runBlocked -> runToBlock = 1;
                    runBlocked -> blockToReady = 0;
                    insertHeapVerbose (runBlocked, v, &verboseCounter);
                }
                removeRoot (heap, &heapCounter, 0);
                insertToHeap (currThread, heap, &heapCounter);
            }
            k++;
        }
        turnaround = (last + first) / 2.0;
        cpuUtilization = (float)totalCpuBurst / (float)totalTime * 100;

        if (argc < 4) {
            /* No flags entered */
            printf("Total Time Required is %d units\n", abs (totalTime));
            printf("Average Turnaround Time is %g units\n", fabs (turnaround));
            printf("CPU Utilization is %g %%\n", fabsf (cpuUtilization));
            return 0;
        }

        if (detailInfo == 1 && verbose == 0) {
            /* Detailed flag only */
            printf("Total Time Required is %d units\n", abs (totalTime));
            printf("Average Turnaround Time is %g units\n", fabs (turnaround));
            printf("CPU Utilization is %g %%\n", fabsf (cpuUtilization));
            printDetailed (finished, &finishCounter);
            return 0;
        }

        if (detailInfo == 0 && verbose == 1) {
            /* Verbose flag only */
            printf("Total Time Required is %d units\n", abs (totalTime));
            printf("Average Turnaround Time is %g units\n", fabs (turnaround));
            printf("CPU Utilization is %g %%\n\n", fabsf (cpuUtilization));
            printVerbose (v, &verboseCounter);
        }

        if (detailInfo == 1 && verbose == 1) {
            /* Detailed and verbose flag */
            printf("Total Time Required is %d units\n", abs (totalTime));
            printf("Average Turnaround Time is %g units\n", fabs (turnaround));
            printf("CPU Utilization is %g %%\n\n", fabsf (cpuUtilization));
            printDetailed (finished, &finishCounter);
            printf ("\n");
            printVerbose (v, &verboseCounter);
        }

        free (heap);
        free (finished);
        free (v);
    }

    return 0;
}
