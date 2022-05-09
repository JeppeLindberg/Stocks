#ifndef STOCKS_TRADING_H
#define STOCKS_TRADING_H

#include "stochastic_oscillator.h"

struct trading_t{
    stochastic_oscillator_t so;

    trading_t(const std::string& file_path): so(file_path)
    {}
};

#endif //STOCKS_TRADING_H
