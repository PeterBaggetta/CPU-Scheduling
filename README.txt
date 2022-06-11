*** Author Information ***

* Author: Pietro Baggetta
* Email: pbaggett@uoguelph.ca
* Student ID: 1092160

*** Course Information ***
* CIS*3110
* Assignment 2: Cpu Scheduling

*** How to Compile ***
    - make all

*** How to clean/remove all .o and executable files ***
    - make clean
    
*** How to run program ***
    - Run these commands after "make all" has been done and the files are compiled
    - All the different possible options for the program
    - First Come First Serve
        > ./simcpu < input_file             -> No flags, simple info
        > ./simcpu -d < input_file          -> Details about the process and threads times
        > ./simcpu -d -v < input_file       -> Details about each threads states
    - Round Robin
        > ./simcpu -r # < input_file        -> No flags, simple info
        > ./simcpu -d -r # < input_file     -> Details about the process and threads times
        > ./simcpu -d -v -r # < input_file  -> Details about each threads states

*** NOTES BEFORE LAUNCHING PROGRAM ***
    - Ensure the file to be read into the program is in the same directory
      as the main program.

*** FLAGS ***
    - Default mode (no flags, "./simcpu < input_file"):
        > The type of scheduling is printied out (First Come First Serve).
        > Total time required to execute the simulated scheduling is shown.
        > Average turnaround time of the simulated scheduling is shown.
        > Percentage of the cpu utilization is shown.
    - Detailed information mode ("./simcpu -d < input_file"):
        > Along with the type of scheduling, total time, turnaround time, and cpu
          utilization, the details about each thread is shown.
        > The thread number and process number are printed out along with the service
          time, I/O time, turnaround time and finish time for the specific thread. This
          is done for all threads.
    - Verbose mode ("./simcpu -v < input_file"):
        > Prints out all the details for the threads and process state changes.
        > Will keep track and print out the different state transitions. New to
          ready, ready to running, running to terminated, running to blocked, and 
          blocked to ready will all be tracked and printed.
    - Round Robin (no flags, "./simcpu -r # < input_file"):
        > The type of scheduling is printied out along with the quantum number
          entered from the command line (Round Robin (quantum = # time units)).
        > Total time required to execute the simulated scheduling is shown.
        > Average turnaround time of the simulated scheduling is shown.
        > Percentage of the cpu utilization is shown.

    - Any combination of the flags can be inputted into the command line.
    - Min input on command line ("./simcpu < input_file").
    - Max input on command line: ("./simcpu -d -v -r # < input_file").
        > flags are to be entered in this order to prevent any confusion.

*** ERRORS IN CODE ***
    - Both modes run with no problems and are able to get input and calculate the
      associated values for whatever flags are entered.
    - Sometimes the output of total time and turnaround time can vary if the input
      file is very large.