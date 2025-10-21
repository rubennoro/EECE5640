#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

//States of Philosopher
#define HUNGRY 0
#define EATING 1
#define THINKING 2
//#define philosophers 5
//Global Var for # of Philosophers
int philosophers;
//Global Vars for the Arrays of Philosophers and Forks
int* phils;
int* forks;

//Number of runs
int runs = 20;

class Moves{
        private:
                //Tracking Philosopher State
                //int philStates[philosophers];

                pthread_cond_t* cond;
                pthread_mutex_t lock;
        public:
                void print_one_state(int phil_num){
                        cout << "Philosopher " << phil_num << ": ";
                        switch(phils[phil_num]){
                                case THINKING:
                                        cout << "is thinking" << endl;
                                        break;
                                case HUNGRY:
                                        cout << "is hungry" << endl;
                                        break;
                                case EATING:
                                        cout << "is eating with forks " << phil_num << " and " << (phil_num+1)%philosophers << endl;
                                        break;
                        }
                        cout << "------------------------------" << endl;
                }

                void print_state(){
                        for(int i = 0; i < philosophers; i++){
                                cout << "Philosopher " << i << ": ";
                                switch(phils[i]){
                                        case THINKING:
                                                cout << "is Thinking" << endl;
                                                break;
                                        case HUNGRY:
                                                cout << "is Hungry" << endl;
                                                break;
                                        case EATING:
                                                cout << "is Eating with forks " << i << " and " << (i+1)%philosophers << endl;
                                                break;
                                }
                                cout << "---------------------" << endl;
                        }


                }
                void check_forks(int phil_num){
                        //Case where both forks are available and person is hungry
                        //cout << phil_num << endl;
                        if(forks[phil_num%philosophers] == 0 && forks[(phil_num+1)%philosophers]==0 && phils[phil_num] == HUNGRY){
                                //cout << phil_num << endl;
                                forks[phil_num%philosophers] = phil_num;
                                forks[(phil_num+1)%philosophers] = phil_num;
                                phils[phil_num] = EATING;
                                pthread_cond_signal(&cond[phil_num]);

                        }
                        else if(forks[(phil_num) % philosophers] == 0 && forks[(phil_num+1)%philosophers] == phil_num && phils[phil_num] == HUNGRY){
                                phils[phil_num] = EATING;
                                forks[(phil_num)%philosophers] == phil_num;
                                pthread_cond_signal(&cond[phil_num]);
                        }
                        else if(forks[(phil_num+1) % philosophers] == 0 && forks[(phil_num)%philosophers] == phil_num && phils[phil_num] == HUNGRY){
                                phils[phil_num] = EATING;
                                forks[(phil_num+1)%philosophers] == phil_num;
                                pthread_cond_signal(&cond[phil_num]);
                        }
                        else if(forks[(phil_num+1) % philosophers] == phil_num && forks[(phil_num)%philosophers] == phil_num && phils[phil_num] == HUNGRY){
                                phils[phil_num] = EATING;
                                pthread_cond_signal(&cond[phil_num]);
                        }
                }
                void grab_fork(int phil_num){
                        //Grab a fork to the left or right of them (random)
                        //cout << phil_num << endl;
                        pthread_mutex_lock(&lock);

                        phils[phil_num] = HUNGRY;
                        int randomPick = rand()%2;
                        if(randomPick == 0){
                                if(forks[phil_num%philosophers] == 0){
                                        forks[phil_num%philosophers] = phil_num;
                                }
                                else if(forks[(phil_num+1)%philosophers] == 0){
                                        forks[(phil_num+1)%philosophers] = phil_num;
                                }
                        }
                        else{
                                if(forks[(phil_num+1)%philosophers] == 0){
                                        forks[(phil_num+1)%philosophers] = phil_num;
                                }
                                else if(forks[phil_num%philosophers] == 0){
                                        forks[phil_num%philosophers] = phil_num;
                                }
                        }

                        //If adjacent philosophers aren't eating, then you can eat
                        check_forks(phil_num);

                        //print_one_state(phil_num);
                        //If the adjacent philosophers were eating, then you couldn't eat, so this is the condition for that
                        if(phils[phil_num] == EATING){

                                cout << "Philosopher " << phil_num << " Eats with forks " << phil_num << " and " << (phil_num +1)%philosophers << endl;

                        }
                        else if (phils[phil_num] != EATING){
                                //Philosopher waits for the signal to eat
                                pthread_cond_wait(&cond[phil_num], &lock);
                                cout << "Philosopher " << phil_num << " Eats with forks " << phil_num << " and " << (phil_num+1)%philosophers << endl;

                        }

                        //cout << phil_num << endl;
                        pthread_mutex_unlock(&lock);
                        //print_state();
                }
                void return_fork(int phil_num){
                        pthread_mutex_lock(&lock);

                        phils[phil_num] = THINKING;
                        forks[phil_num] = 0;
                        forks[(phil_num+1)%philosophers] = 0;
                        //Let other philosophers have chance to eat
                        if(phil_num == 0){
                                check_forks((phil_num+1)%philosophers);
                                check_forks(philosophers-1);
                        }
                        else{
                                check_forks(((phil_num+1)%philosophers));
                                check_forks((phil_num-1)%philosophers);
                        }
                        //print_state();
                        pthread_mutex_unlock(&lock);

                }
                Moves(int numPhils){
                        cond = new pthread_cond_t[numPhils];
                        for(int i = 0; i < philosophers; i++){
                                pthread_cond_init(&cond[i], NULL);
                                phils[i] = THINKING;
                        }
                        pthread_mutex_init(&lock, NULL);

                }
                ~Moves(){

                        delete[] cond;
                        pthread_mutex_destroy(&lock);
                }

};

Moves phil_choices(philosophers);

void* philMove(void *arg){

        int runs = 0;
        int i = *(int *)arg;
        //cout << i << endl;
        while(runs < 20){
                //int i = *(int *)arg;
                sleep(5);
                //Need to grab a fork
                phil_choices.grab_fork(i);
                //Simulates eating
                sleep(5);
                //Put forks back if eating
                phil_choices.return_fork(i);
                runs++;
        }

        return NULL;
}
int main(int argc, char* argv[]){

    if(argc < 2){
            cout << "Must Input Odd Number of Philosophers in Command Line";
            return 1;
    }
    philosophers = atoi(argv[1]);

    phils = (int*)malloc(sizeof(int)*philosophers);
    forks = (int*)malloc(sizeof(int)*philosophers);
    //Initializing Numbers for Each Philosophers
    //Setting all Forks to Free
    for(int i = 0; i < philosophers; i++){
            phils[i] = THINKING;
            forks[i] = 0;
    }

    pthread_t threads[philosophers];
    pthread_attr_t thread_attr;

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    for(int i = 0; i < philosophers; i++){
            pthread_create(&threads[i], &thread_attr, philMove, &i);
            cout << "Philosophers " << i << " began thinking..." << endl;
    }
    for(int i = 0; i < philosophers; i++){
            pthread_join(threads[i], NULL);
    }
    pthread_attr_destroy(&thread_attr);
    pthread_exit(NULL);
    delete[] phils;
    delete[] forks;

    return 0;
}