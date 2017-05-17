#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include "timeit.h"
#define DEFAULT_NUM_FORK 100

void do_pthread(int num);
void do_fork(int num);

using namespace std;

int main(int argc, char **argv){
    int num_fork;
    if(argc == 1){
        num_fork = DEFAULT_NUM_FORK;
    }
    else{
        num_fork = atoi(argv[1]);
    }
    timeit t;
    t.start();
    do_fork(num_fork);
    t.record();
    cout << num_fork << " processes forked, " << endl;
    t.print_last();
    t.start();
    do_pthread(num_fork);
    t.record();
    cout << num_fork << " threads created, " << endl;
    t.print_last();
    return 0;
}

void do_fork(int num){
    for(int i=0;i<num;i++){
        pid_t pid;
        pid = fork();
        if(!pid)
            exit(0);
    }
}

void * dumb(void * x){
    return NULL;
}

void do_pthread(int num){
    pthread_t threads[num];
    for(int i=0;i<num;i++){
        pthread_create(&(threads[i]),NULL,&dumb,NULL);
    }
}