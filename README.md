# Memory-Management
Page Replacement Algorithms

We will be simulating using two different page replacement algorithms and evaluating the performance of each. These two algorithms will be PFF and VSWS.  For this we will simulate a system where the OS has available an infinite amount of physical memory.  However, we don’t want to over allocate memory to processes so we will do demand paging with the processes in the system.
The file contains integers only, one per line.  The first integer is an indication for how many pages the process occupies; this will not change throughout the run of the program.  All subsequent integers are page references which the program makes during it execution.
In the PFF algorithm, we decide if a new frame should be allocated based on the time since the last page fault. For this, we can use a number of page accesses.  
In the VSWS algorithm, we have M,L and Q to help us decide when to run the algorithm.  

