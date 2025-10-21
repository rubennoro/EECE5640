This program, Q2.cpp, models the eating philosopher's problem made famous by Djikstra.

Details on Implementation:
This program uses dynamically allocated forks and phils arrays. The forks array stores either 0's
or phil_num's, where 0's represent an open fork and phil_num's represents a fork taken by a particular 
philosopher.
Initially, the user sets the number of philosophers and each are assigned a thread to run philMove.
Between 5 seconds of delay, each philosopher has the opportunity to grab a fork (grab_fork()) or two by acquiring a mutex. They then change their state to HUNGRY, and EATING if they are able to acquire two forks. If they aren't they wait for a signal to acquire their next fork. This takes place with the check_fork() function which checks if it's possible for the specific philosopher to acquire both forks, or just one more to have the two necessary to eat. After 5 seconds, the philosophers EATING will return their forks, and check_fork() will be applied to adjacent philosophers to determine if they are now able to begin eating.
ISSUES:
There existed issues in the print statements of the code that I was not able to fully solve. My current implementation prints out if a philosopher is eating every 5 seconds based on the delay.
//print_state(); in Line 136 may be uncommented to determine which philosophers are EATING or HUNGRY
//print_one_state(phil_num); in Line 120 may be uncommented to determine which philosophers are EATING or HUNGRY as well
//print_state(); in Line 153 may be uncommented, as it shows when a philosopher is giving up their forks if new philosophers acquire them and begin eating, printing the states of all philosophers.

To run:
g++ -pthread Q2.cpp -o Q2
./Q2 <odd_num_philosophers>

The program will output something like this:
Example: 5 philosophers

[rnoroian@xi Q2]$ ./Q2 5
Philosophers 0 began thinking...
Philosophers 1 began thinking...
Philosophers 2 began thinking...
Philosophers 3 began thinking...
Philosophers 4 began thinking...
Philosopher 1 Eats with forks 1 and 2
Philosopher 2 Eats with forks 2 and 3
Philosopher 1 Eats with forks 1 and 2
Philosopher 3 Eats with forks 3 and 4
Philosopher 2 Eats with forks 2 and 3
Philosopher 4 Eats with forks 4 and 0
Philosopher 1 Eats with forks 1 and 2
Philosopher 3 Eats with forks 3 and 4
Philosopher 2 Eats with forks 2 and 3
Philosopher 4 Eats with forks 4 and 0