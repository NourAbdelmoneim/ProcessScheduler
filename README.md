# Process Scheduler

This program takes a number of processes with 2 running times and 2 blocking times each as input and runs either round-robin scheduling or first-in first-out (FIFO) scheduling. 

Compile with this command: g++ -o sched scheduling.cpp

Command line arguments should be inputted in the following order:
1 - number of processes
2 - scheduling algorithm (1 for FIFO and 2 for round-robin)
3 - running time 1 of first process
4 - running time 2 of first process
5 - blocking time 1 of first process
6 - blocking time 2 for first process

And so on for remaining processes 

Input command line arguments and run as such: ./sched 2 2 1 3 2 3 2 3 2 3 2

The program will output the state of each process at the end of every cycle.
