#include "timeit.h"

using namespace std;

    void timeit::start(){
        start_clock = clock();
        gettimeofday(&start_time,NULL);
    }

    void timeit::record(){
        clock_stamp.push_back(clock());
        struct timeval temp;
        gettimeofday(&temp,NULL);
        time_stamp.push_back(temp);
    }

    void timeit::print_last(){
        unsigned long sec,usec;
        int num = clock_stamp.size();
        if(!num)
            return;
        num--;
        sec = time_stamp[num].tv_sec - start_time.tv_sec;
        usec = time_stamp[num].tv_usec - start_time.tv_usec;
        double time_elap = sec * 1000 + usec/1000;
        cout << "Real time elapsed: " << time_elap << "ms."<<endl;
        cout << "Process clock time elapsed: " << (clock_stamp[num] - start_clock) / (double)(CLOCKS_PER_SEC / 1000) << "ms." <<endl;
    }