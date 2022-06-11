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

/* Find the parent node */
int parentIndex (int index) {
    return (index - 1) / 2;
}

/* Finds the left most node in the tree */
int leftIndex (int index) {
    return index * 2 + 1;
}

/* Finds the right most node in the tree */
int rightIndex (int index) {
    return index * 2 + 2;
}

/* Heapify / sort the heap recursively using minimum heap algorithm */
void heapify (Thread **heap, int *heapCounter, int index) {

    if (*heapCounter <= 1) {
        /* Heap is empty */
        return;
    }
    int left = leftIndex (index);
    int right = rightIndex (index);
    int smallest = index;

    /* Find the left most node and gets the smallest index to get replaced */
    if (left <= *heapCounter && heap[left] -> arrivalTime <= heap[index] -> arrivalTime) {
        if (heap[left] -> arrivalTime == heap[index] -> arrivalTime) {
            if (heap[left] -> processNum <= heap[index] -> processNum) {
                if (heap[left] -> processNum == heap[index] -> processNum) {
                    if (heap[left] -> threadNum <= heap[index] -> threadNum) {
                        if (heap[left] -> threadNum != heap[index] -> threadNum) {
                            smallest = left;
                        }
                    }
                } else {
                    smallest = left;
                }
            }
        } else {
            smallest = left;
        }
    } else {
        smallest = index;
    }
    /* Find the right most node and gets the smallest index to get replaced */
    if (right <= *heapCounter && heap[right] -> arrivalTime <= heap[smallest] -> arrivalTime) {
        if (heap[right] -> arrivalTime == heap[index] -> arrivalTime) {
            if (heap[right] -> processNum <= heap[index] -> processNum) {
                if (heap[right] -> processNum == heap[index] -> processNum) {
                    if (heap[right] -> threadNum <= heap[index] -> threadNum) {
                        if (heap[left] -> threadNum != heap[index] -> threadNum) {
                            smallest = right;
                        }
                    }
                } else {
                    smallest = right;
                }
            }
        } else {
            smallest = right;
        }
    }
    /* Replaces the index with the smallest (puts new node at bottom of tree) */
    if (smallest != index) {
        Thread *temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        heapify (heap, heapCounter, smallest);
    }
}

/* Heapify / sort the heap recursively using minimum heap algorithm */
void verboseHeapify (VerboseInfo **heap, int *heapCounter, int index) {

    if (*heapCounter <= 1) {
        return;
    }
    int left = leftIndex (index);
    int right = rightIndex (index);
    int smallest = index;

    /* Find the left most node and gets the smallest index to get replaced */
    if (left <= *heapCounter && heap[left] -> time <= heap[index] -> time) {
        if (heap[left] -> time == heap[index] -> time) {
            if (heap[left] -> process <= heap[index] -> process) {
                if (heap[left] -> process == heap[index] -> process) {
                    if (heap[left] -> thread <= heap[index] -> thread) {
                        if (heap[left] -> thread != heap[index]-> thread) {
                            smallest = left;
                        }
                    }
                } else {
                    smallest = left;
                }
            }
        } else {
            smallest = left;
        }
    } else {
        smallest = index;
    }
    /* Find the right most node and gets the smallest index to get replaced */
    if (right <= *heapCounter && heap[right]->time <= heap[smallest]->time) {
        if (heap[right] -> time == heap[index] -> time) {
            if (heap[right] -> process <= heap[index] -> process) {
                if (heap[right] -> process == heap[index] -> process) {
                    if (heap[right] -> thread <= heap[index] -> thread) {
                        if (heap[left] -> thread != heap[index] -> thread) {
                            smallest = right;
                        }
                    }
                } else {
                    smallest = right;
                }
            }
        } else {
            smallest = right;
        }
    }
    /* Replaces the index with the smallest (puts new node at bottom of tree) */
    if (smallest != index) {
        VerboseInfo *temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        verboseHeapify (heap, heapCounter, smallest);
    }
}

/* Insert the node to the heap */
void insertToHeap (Thread *currThread, Thread **heap, int *heapCounter) {

    heap[*heapCounter] = currThread;
    *heapCounter = *heapCounter + 1;
    int index = *heapCounter - 1;

    /* Places new node at the child of the parent */
    while (index > 0 && heap[parentIndex (index)] -> arrivalTime >= heap[index]->arrivalTime) {
        if (heap[parentIndex (index)] -> arrivalTime == heap[index] -> arrivalTime) {
            if (heap[parentIndex( index)] -> threadNum >= heap[index] -> threadNum) { 
                if (heap[parentIndex (index)] -> threadNum == heap[index] -> threadNum) {
                    break;
                } else {
                    /* Replace node */
                    Thread *temp = heap[parentIndex (index)];
                    heap[parentIndex (index)] = heap[index];
                    heap[index] = temp;
                    index = parentIndex (index);
                }
            } else {
                break;
            }  
        } else {
            /* Replace node */
            Thread *temp = heap[parentIndex (index)];
            heap[parentIndex (index)] = heap[index];
            heap[index] = temp;
            index = parentIndex (index);
        }
    }
}

/* Insert the node to the heap */
void insertHeapVerbose (VerboseInfo *currThread, VerboseInfo **heap, int *heapCounter) {

    heap[*heapCounter] = currThread;
    *heapCounter = *heapCounter + 1;

    int index = *heapCounter - 1;

    /* Place the information in the heap in decenting order of threads and processes */
    while (index > 0 && heap[parentIndex (index)] -> time >= heap[index] -> time) {
        if (heap[parentIndex (index)] -> time == heap[index] -> time) {
            if (heap[parentIndex( index)] -> process >= heap[index] -> process) {
                if (heap[parentIndex (index)] -> process == heap[index] -> process) {
                    if (heap[parentIndex (index)] -> thread >= heap[index] -> thread) {
                        if (heap[parentIndex (index)] -> thread == heap[index] -> thread) {
                            break;
                        } else {
                            /* Replace node if smaller */
                            VerboseInfo *temp = heap[parentIndex (index)];
                            heap[parentIndex (index)] = heap[index];
                            heap[index] = temp;
                            index = parentIndex (index);
                        }
                    } else {
                        break;
                    }
                } else {
                    /* Replace node if smaller */
                    VerboseInfo *temp = heap[parentIndex (index)];
                    heap[parentIndex (index)] = heap[index];
                    heap[index] = temp;
                    index = parentIndex (index);
                }
            } else {
                break;
            } 
        } else {
            /* Replace node if smaller */
            VerboseInfo *temp = heap[parentIndex (index)];
            heap[parentIndex (index)] = heap[index];
            heap[index] = temp;
            index = parentIndex (index);
        }
    }
}

/* Get the finished heap */
void heapFinished (Thread *thread, Thread **heap, int *heapCounter) {
    
    heap[*heapCounter] = thread;
    *heapCounter = *heapCounter + 1;
    int index = *heapCounter - 1;

    /* Reorder the heap to be in order for printing */
    while (index > 0 && heap[parentIndex (index)] -> arrivalTime >= heap[index] -> arrivalTime) {
        if (heap[parentIndex (index)] -> arrivalTime == heap[index] -> arrivalTime) {
            if (heap[parentIndex (index)] -> threadNum >= heap[index] -> threadNum) {
                if (heap[parentIndex (index)]->threadNum == heap[index] -> threadNum) {
                    break;
                } else {
                    /* Replace node */
                    Thread *temp = heap[parentIndex (index)];
                    heap[parentIndex (index)] = heap[index];
                    heap[index] = temp;
                    index = parentIndex (index);
                }
            } else {
                break;
            }
        } else {
            /* Replace node */
            Thread *temp = heap[parentIndex (index)];
            heap[parentIndex (index)] = heap[index];
            heap[index] = temp;
            index = parentIndex (index);
        }
    }
}

/* Remove the root node */
void removeRoot (Thread **heap, int *heapCounter, int num) {
    
    Thread *temp = heap[*heapCounter - 1];
    /* replace root with last element */
    heap[0] = temp;

    *heapCounter = *heapCounter - 1;

    heapify (heap, heapCounter, 0);
}

/* Remove the root node */
void verboseRemoveRoot (VerboseInfo **heap, int *heapCounter, int num) {
    VerboseInfo *temp = heap[*heapCounter - 1];
    /* replace root with last element */
    heap[0] = temp;

    *heapCounter = *heapCounter - 1;

    verboseHeapify (heap, heapCounter, 0);
}
