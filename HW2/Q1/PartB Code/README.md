There are 3 Programs in this Folder.

1.
In the Q1sequent.c program, the number of darts and number of threads are user inputs. There are no threads.

TO RUN MONTE CARLO:
gcc Q1sequent.c -o Q1seq
./Q1pthread <num_of_darts>

OUTPUT:
Output includes the time taken, in milliseconds, and the Pi estimation.

2. Monte Carlo OpenMP
In the Q1carloMP.c program, the number of darts and number of threads are user inputs. An OpenMP pragma is employed in this case.

TO RUN MONTE CARLO:
gcc -o CarloMP Q1carloMP.c -fopenmp -lm
./CarloMP <num_of_darts> <num_threads>

OUTPUT:
Output includes the time taken, in milliseconds, and the Pi estimation.

3. Leibniz OpenMP
In the Q1leibMP.c program, the number of darts and number of threads are user inputs. An OpenMP pragma is employed in this case.

TO RUN MONTE CARLO:
gcc -o LeibMP Q1leigMP.c -fopenmp -lm
./LeibMP <num_of_darts> <num_threads>

OUTPUT:
Output includes the time taken, in milliseconds, and the Pi estimation.
