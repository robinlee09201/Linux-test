#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <vector>

class timeit{

private:
    //member
    //  time start
    //      process clock
    std::clock_t start_clock;
    //      real time
    struct timeval start_time;
    //  time stamps
    std::vector<std::clock_t> clock_stamp;
    std::vector<struct timeval> time_stamp;

public:
    //member functions
    void start();
    void record();
    void print_last();
    void print_all();
}