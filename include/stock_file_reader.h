#ifndef STOCKS_STOCK_FILE_READER_H
#define STOCKS_STOCK_FILE_READER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "json.h"
#include "utility.h"

class trade_json_t;
std::ostream& operator<<(std::ostream& os, const std::vector<trade_json_t>& trades);
std::ostream& operator<<(std::ostream& os, const trade_json_t& trade);
std::ostream& operator<<(std::ostream& os, const std::tm& time);

class trade_json_t
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

    friend bool operator==(const trade_json_t& t1, const trade_json_t& t2)
    {
        return (t1.time == t2.time) &&
               (t1.price == t2.price) &&
               (t1.amount == t2.amount) &&
               (t1.buyer == t2.buyer) &&
               (t1.seller == t2.seller) &&
               (t1.seq == t2.seq) &&
               (t1.code == t2.code);
    }

    friend std::ostream& operator<<(std::ostream& os, const trade_json_t& trade) {
        return os << "{ \"time\": \"" << trade.time << "\", " <<
                  "\"price\": " << trade.price << ", " <<
                  "\"amount\": " << trade.amount << ", " <<
                  "\"buyer\": \"" << trade.buyer << "\", " <<
                  "\"seller\": \"" << trade.seller << "\", " <<
                  "\"seq\": " << trade.seq << ", " <<
                  "\"code\": " << trade.code << "} ";
    }

    trade_json_t():time(), price(), amount(), buyer(), seller(),seq(), code(){}

    trade_json_t(std::string _time, double _price, int _amount,
                 std::string _buyer, std::string _seller, int _seq, int _code):
            time(), price(_price), amount(_amount), buyer(_buyer), seller(_seller),
            seq(_seq), code(_code){
        set_time(_time);
    }
};

// Nested datastructure for JSON data
struct stock_json_t
{
    std::string tag;
    std::string isin;
    int shares;
    std::string name;
    std::string url;
    std::string market;
    std::string sector;
    std::string segment;
    std::vector<trade_json_t> trades;

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

    friend bool operator==(const stock_json_t& s1, const stock_json_t& s2)
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

std::ostream& operator<<(std::ostream& os, const stock_json_t& stock) {
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

std::ostream& operator<<(std::ostream& os, const std::vector<trade_json_t>& trades) {
    os << "[";
    auto first = true;
    for (const trade_json_t& trade : trades) {
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

public:
    stock_json_t read_trades(int amount, int offset){
        stock_json_t jd;

        ifs >> json_t{jd, amount, offset};

        return jd;
    }

    stock_file_reader_t(const std::string& path){
        ifs = std::ifstream(path, std::ifstream::in);
    }
    ~stock_file_reader_t(){
        ifs.close();
    }
};

#endif //STOCKS_STOCK_FILE_READER_H
