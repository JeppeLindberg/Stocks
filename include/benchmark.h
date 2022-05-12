#ifndef STOCKS_BENCHMARK_H
#define STOCKS_BENCHMARK_H

#include <chrono>

using namespace std::chrono;

class benchmark_t{
private:
    system_clock::time_point start_time;

public:
    double get_time(){
        system_clock::time_point now = high_resolution_clock::now();
        return duration<double, std::milli>(now - start_time).count();
    }

    benchmark_t(){
        start_time = high_resolution_clock::now();
    }
};
#endif //STOCKS_BENCHMARK_H
