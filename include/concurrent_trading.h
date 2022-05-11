
#ifndef STOCKS_CONCURRENT_TRADING_H
#define STOCKS_CONCURRENT_TRADING_H

#include <future>
#include "trading.h"

struct concurrent_trading_t{
    std::tm start_time;
    std::string file_path;
    int simulation_period;
    std::vector<std::pair<std::string, std::function<Action(point_t)>>> thread_prototypes;
    std::mutex output_mutex;

    static void consumer(std::function<Action(point_t)>& action,
                         const std::string& name,
                         const std::tm& _start_time,
                         const std::string& _file_path,
                         const int& _simulation_period,
                         std::mutex& _output_mutex){
        trading_t trading{_file_path, _start_time};
        trading.trade_strategy = action;
        trading.simulate_trades(_simulation_period);

        std::lock_guard<std::mutex> guard(_output_mutex);
        std::cout << name << "\n" << trading.result_as_string() << trading.money << std::endl;
    }

    void join(){
        std::vector<std::thread> threads(thread_prototypes.size());
        for (auto& tp : thread_prototypes){
            std::promise<std::string> prom;
            std::future<std::string> fut = prom.get_future();
            std::thread new_thread = std::thread(consumer,
                                                 std::ref(tp.second),
                                                 std::ref(tp.first),
                                                 std::ref(start_time),
                                                 std::ref(file_path),
                                                 std::ref(simulation_period),
                                                 std::ref(output_mutex));
            threads.push_back(std::move(new_thread));
        }

        for (auto &thread: threads) {
            if (thread.joinable())
                thread.join();
        }
    }

    concurrent_trading_t(std::tm _start_time, std::string _file_path, int _simulation_period):
        start_time(_start_time), file_path(_file_path), simulation_period(_simulation_period)
    {}
};

#endif //STOCKS_CONCURRENT_TRADING_H
