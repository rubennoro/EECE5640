The q1.cpp program compares the runtimes of the taylor series 
for sine(x) where x is a value inputted by the user. It compares the speed of 
single precision and double precision computations.

Important Variables:
In this code, the variables used for experimentation were X and NUMS.
X was used to change the value in the sine(X)
NUMS was used to change the number of series approximations to compare runtime.

To run:
g++ -std=c++17 q1.cpp -o q1
./q1

Example Output:
Float Time 0.0662761
Float Result 0.989353
Double Time 0.0083971
Double Result 0.989354