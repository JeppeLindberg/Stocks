#ifndef STOCKS_STOCK_FILE_READER_H
#define STOCKS_STOCK_FILE_READER_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <set>
#include "json.h"
#include "utility.h"

class trade_t;
std::ostream& operator<<(std::ostream& os, const std::set<trade_t>& trades);
std::ostream& operator<<(std::ostream& os, const trade_t& trade);

class trade_t
{
private:
    void set_time(std::string& time_str){
        std::istringstream ss(time_str);
        ss >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S.000+0100");
    }

public:
    std::tm time;
    double price;
    int amount;
    std::string buyer;
    std::string seller;
    int seq;
    int code;

    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
        std::string time_str;

        visit("time", time_str);
        visit("price", price);
        visit("amount", amount);
        visit("buyer", buyer);
        visit("seller", seller);
        visit("seq", seq);
        visit("code", code);

        set_time(time_str);
    }

    friend bool operator==(const trade_t& t1, const trade_t& t2)
    {
        return (t1.time == t2.time) &&
               (t1.price == t2.price) &&
               (t1.amount == t2.amount) &&
               (t1.buyer == t2.buyer) &&
               (t1.seller == t2.seller) &&
               (t1.seq == t2.seq) &&
               (t1.code == t2.code);
    }

    friend bool operator<(const trade_t& t1, const trade_t& t2)
    {
        if (t1.time != t2.time)
            return (t1.time < t2.time);
        if (t1.price != t2.price)
            return (t1.price < t2.price);
        if (t1.amount != t2.amount)
            return (t1.amount < t2.amount);
        if (t1.buyer != t2.buyer)
            return (t1.buyer < t2.buyer);
        if (t1.seller != t2.seller)
            return (t1.seller < t2.seller);
        if (t1.seq != t2.seq)
            return (t1.seq < t2.seq);
        if (t1.code != t2.code)
            return (t1.code < t2.code);
        return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const trade_t& trade) {
        return os << "{ \"time\": \"" << trade.time << "\", " <<
                  "\"price\": " << trade.price << ", " <<
                  "\"amount\": " << trade.amount << ", " <<
                  "\"buyer\": \"" << trade.buyer << "\", " <<
                  "\"seller\": \"" << trade.seller << "\", " <<
                  "\"seq\": " << trade.seq << ", " <<
                  "\"code\": " << trade.code << "} ";
    }

    trade_t(): time(), price(), amount(), buyer(), seller(), seq(), code(){}

    trade_t(std::string _time, double _price, int _amount,
            std::string _buyer, std::string _seller, int _seq, int _code):
            time(), price(_price), amount(_amount), buyer(_buyer), seller(_seller),
            seq(_seq), code(_code){
        set_time(_time);
    }
};

// Nested datastructure for JSON data
struct stock_t
{
    std::string tag;
    std::string isin;
    int shares;
    std::string name;
    std::string url;
    std::string market;
    std::string sector;
    std::string segment;
    std::set<trade_t> trades;

    void extend(stock_t& other_stock) {
        using namespace std::literals::string_literals;

        if (tag != other_stock.tag)
            throw std::logic_error("Attempting to extend stock "s + tag + " with stock " + other_stock.tag);

        for(const trade_t& trade: other_stock.trades){
            trades.insert(trade);
        }
    }

    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
        visit("tag", tag);
        visit("isin", isin);
        visit("shares", shares);
        visit("name", name);
        visit("url", url);
        visit("market", market);
        visit("sector", sector);
        visit("segment", segment);
        visit("trades", trades);
    }

    friend bool operator==(const stock_t& s1, const stock_t& s2)
    {
        return (s1.tag == s2.tag) &&
                (s1.isin == s2.isin) &&
                (s1.shares == s2.shares) &&
                (s1.name == s2.name) &&
                (s1.url == s2.url) &&
                (s1.market == s2.market) &&
                (s1.sector == s2.sector) &&
                (s1.segment == s2.segment) &&
                (s1.trades == s2.trades);
    }
};

std::ostream& operator<<(std::ostream& os, const stock_t& stock) {
    return os << "{ \"tag\": \"" << stock.tag << "\", " <<
              "\"isin\": \"" << stock.isin << "\", " <<
              "\"shares\": " << stock.shares << ", " <<
              "\"name\": \"" << stock.name << "\", " <<
              "\"url\": \"" << stock.url << "\", " <<
              "\"market\": \"" << stock.market << "\", " <<
              "\"sector\": \"" << stock.sector << "\", " <<
              "\"segment\": \"" << stock.segment << "\"," <<
              "\"trades\": " << stock.trades << "}";
}

std::ostream& operator<<(std::ostream& os, const std::set<trade_t>& trades) {
    os << "[";
    auto first = true;
    for (const trade_t& trade : trades) {
        if (!first)
            os << ",";
        else
            first = false;
        os << trade;
    }
    return os << "]";
}

class stock_file_reader_t{
    std::ifstream ifs;
    bool first_read = true;
    stock_t sj;
    std::string path;

    stock_t read_trades(int amount, int offset){
        ifs = std::ifstream(path, std::ifstream::in);
        if (first_read){
            sj = stock_t{};
            ifs >> json_t{sj, amount, offset};
            first_read = false;
        }
        else
        {
            stock_t new_sj{};
            ifs >> json_t{new_sj, amount, offset};
            sj.extend(new_sj);
        }
        ifs.close();
        return sj;
    }

public:
    int page_size = 5000;

    stock_t read_page(int page){
        return read_trades(page_size, page * page_size);
    }

    stock_t read_first_trade(){
        return read_trades(1, 0);
    }

    stock_file_reader_t(const std::string& file_path){
        path = file_path;
        sj = stock_t{};
        first_read = true;
    }
    ~stock_file_reader_t(){
        ifs.close();
    }
};

#endif //STOCKS_STOCK_FILE_READER_H
