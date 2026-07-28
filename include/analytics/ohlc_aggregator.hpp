#pragma once
#include "analytics/ohlc.hpp"
#include "market/market_event.hpp"
#include<unordered_map>
#include<chrono>
#include<iostream>
#include<fstream>


class OHLCAggregator{
private:

    std::unordered_map<std::string, OHLC> current_candles;

public:

    std::ofstream ohlc_file;
    std::chrono::seconds interval_;
    explicit OHLCAggregator(std::chrono::seconds interval);
    void processTrade(const MarketEvent& event);
    void finalizeCandle(const OHLC& candle);
};