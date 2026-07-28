#pragma once
#include<chrono>
#include<cstdint>
#include<string>

// OHLC-> open high low close

struct OHLC{
    std::string symbol;

    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;

    double volume = 0.0;
    std::uint64_t trade_count;

    std::chrono::system_clock::time_point minute_start;  // the starting minute with this price

};