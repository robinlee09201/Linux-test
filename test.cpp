#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "timeit.h"
#define DEFAULT_NUM_FORK 1000
#define RESULT_ON 0

void do_pthread(int num);
void do_fork(int num);

static int num_fork;
static int test_type;
static int tester_ret;

using namespace std;

#if RESULT_ON
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
#else
int main(int argc, char **argv){
    if(argc == 1){
        num_fork = DEFAULT_NUM_FORK;
    }
    else{
        num_fork = atoi(argv[1]);
    }
    cout << "Main process PID: " << getpid() << endl;
    while(1){
        cout << "Enter 0 for fork test, 1 for thread test.\n";
        cin >> test_type;
        cout << "test type: " << test_type <<endl;
        pid_t pid = fork();
        if(!pid){
            //child
            sigset_t waitset;
            sigemptyset(&waitset);
            sigaddset(&waitset,SIGUSR1);
            sigprocmask(SIG_BLOCK,&waitset,NULL);
            int sig;
            int result = sigwait(&waitset,&sig);
            if(result){
                exit(0);
            }
            switch (test_type){
                case 0:{
                    do_fork(num_fork);
                    return 0;
                }
                case 1:{
                    do_pthread(num_fork);
                    return 0;
                }
                default:
                    return 1;
            }
        }
        else if(pid >= 1){
            //parent
            cout << "Tester PID: " << pid << endl;
            cout << "Attach strace and enter any char to continue\n";
            char dumb;
            cin >> dumb;
            kill(pid,SIGUSR1);
            waitpid(pid, &tester_ret,0);
            if(tester_ret == 0){
                switch(test_type){
                    case 0:{
                        cout << "Fork " << num_fork << " process successfully\n";
                        break;
                    }
                    case 1:{
                        cout << "Create " << num_fork << " threads successfully\n";
                        break;
                    }
                    default:    {
                        cout << "Invalid test type\n" ;
                        break;}
                }
            }
            else{
                cout << "test failed! returned: "<<tester_ret<<endl;
            }
            cout << "\n*******************************************************\n\n";
        }
    }
    
    return 0;
}
#endif

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