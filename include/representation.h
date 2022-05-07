#ifndef STOCKS_REPRESENTATIONS_H
#define STOCKS_REPRESENTATIONS_H

#include "stock_file_reader.h"
#include "utility.h"
#include <ctime>
#include <vector>
#include <limits>
#include <map>

struct interval_t{
    std::set<trade_t> trades;
    double first_price;
    double last_price;
    double min_price;
    double max_price;

    void generate_values(){
        using namespace std::literals::string_literals;
        if (trades.empty())
            throw std::logic_error("The interval needs trades to generate values"s);

        first_price = trades.begin()->price;
        last_price = trades.rbegin()->price;
        min_price = std::numeric_limits<int>::max();
        max_price = std::numeric_limits<int>::min();

        for(const trade_t& trade : trades){
            if(trade.price < min_price)
                min_price = trade.price;
            if(trade.price > max_price)
                max_price = trade.price;
        }
    }
};

// Requirement 3
class representation_candle_t{
    stock_file_reader_t sfr;
    std::map<std::tm, interval_t> intervals;
    std::vector<int> pages_fetched;

public:
    int hpc = 24; // Hours per candle
    stock_t stock;

    void get_data(){
        int last_page = -1;
        for (int x : pages_fetched)
            if(x > last_page)
                last_page = x;

        int target_page = last_page + 1;
        stock = sfr.read_page(target_page);
        pages_fetched.push_back(target_page);

        bool first_loop = true;
        interval_t new_interval{};
        std::tm first_key{};
        std::tm prev_key{};

        for(const trade_t& trade : stock.trades){
            std::tm new_key = utility_t::tm_to_key(trade.time, hpc);
            if (first_loop) {
                first_key = new_key;
                first_loop = false;
            }
            if (first_key != new_key)
            {
                if (!new_interval.trades.empty() && new_key != prev_key && !intervals.contains(prev_key))
                {
                    new_interval.generate_values();
                    intervals[prev_key] = new_interval;
                    new_interval = interval_t{};
                }
                if (!intervals.contains(new_key))
                {
                    if (!intervals.contains(new_key)) {
                        new_interval.trades.insert(trade);
                    }
                }
            }
            prev_key = new_key;
        }
    }

    representation_candle_t(const std::string& file_path): sfr{file_path}, intervals(), pages_fetched()
    {}

    representation_candle_t(const std::string& file_path, int page_size): sfr{file_path}, intervals(), pages_fetched()
    {
        sfr.page_size = page_size;
    }
};

#endif //STOCKS_REPRESENTATIONS_H
