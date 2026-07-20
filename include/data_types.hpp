#pragma once
#include<cstdint>

struct ohlc{
    double open;
    double high;
    double low;
    double close;
    uint64_t timestamp;
};

