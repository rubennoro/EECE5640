The code in the folder PartA shows the Pthread implementation of the dart throwing problem
in Homework 2, question 1. 

In the Q1carlo.c program, the number of darts and number of threads are user inputs.
Each thread runs (# darts / # threads) total darts through a sequential for loop.

TO RUN MONTE CARLO:
gcc -pthread Q1carlo.c -o Q1carlo
./Q1pthread <num_of_darts> <num_of_threads>

OUTPUT:
Output includes the time taken for the parallelization, in milliseconds, and the Pi estimation.

In the Q1leib.c program, the number of points and number of threads are user inputs. Each thread runs (# points/ # threads) and sums part of the series through a sequential loop.

TO RUN LEIBNIZ:
gcc -pthread -lm q1elib.c -o q1leib
./Q1leib <num_of_points> <num_of_threads>

OUTPUT:
Output includes the time taken for the parallelization, in milliseconds, and the Pi estimation.

For optimal use, select a number of threads within the bounds of your CPU's cores.