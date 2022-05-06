#ifndef STOCKS_STOCK_FILE_READER_H
#define STOCKS_STOCK_FILE_READER_H

#include <iostream>
#include <fstream>
#include "json.h"

class stock_file_reader_t{
    std::ifstream ifs;

public:
    void read_trades(const int amount, const int offset){
        auto jd = json_data_t{};
        ifs >> json_t{jd};
    }

    stock_file_reader_t(const std::string& path){
        ifs = std::ifstream(path, std::ifstream::in);
    }
    ~stock_file_reader_t(){
        ifs.close();
    }
};

#endif //STOCKS_STOCK_FILE_READER_H
