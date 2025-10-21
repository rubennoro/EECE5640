#include <iostream>
#include <cmath>
#include <time.h>
#include <stdio.h>

//Modify these values to change the sin(x) approximation and the number of terms used for the approximation
#define X 8
#define NUMS 40


//The float functions for factorial and taylor series
float fact(int n){
        float f = 1;
        for(int i = 1; i <= n; i++){
                f *= i;
        }
        return f;
}
float taylor_float(int terms){
        float approx = 0;
        for(int i = 0; i < terms; i++){
                approx += (pow(-1, i) / fact(2 * i + 1)) * pow(X, 2 * i + 1);
        }
        return approx;
}

//The double functions for factorial and taylor series
double fact1(int n){
        double f = 1;
        for(int i = 1; i <= n; i++){
                f *= i;
        }
        return f;
}
double taylor_double(int terms){
        float approx = 0;
        for(int i = 0; i < terms; i++){
                approx += (pow(-1, i) / fact1(2 * i + 1)) * pow(X, 2 * i + 1);
        }
        return approx;
}

//Timer function
double CLOCK(){
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        return (t.tv_sec * 1000) + (t.tv_nsec*1e-6);
}

int main(){
        double start, end;
        start = CLOCK();
        float final = taylor_float(NUMS);
        end = CLOCK();
        std::cout << end - start << std::endl;
        std::cout << final << std::endl;

        start = CLOCK();
        double final1 = taylor_double(NUMS);
        end = CLOCK();
        std::cout << end - start << std::endl;
        std::cout << final1 << std::endl;
        return 0;
}