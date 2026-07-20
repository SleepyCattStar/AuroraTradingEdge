#pragma once
#include<string>
#include<chrono>

enum class Exchange{
    UNKNOWN,
    BINANCE,
    DELTA_EXCHANGE,
    COINBASE,
    MOCK
};
enum class EventType{
    UNKNOWN,
    TRADE,
    BID_UPDATE,
    ASK_UPDATE
};

struct MarketEvent{
    std::string symbol;
    std::chrono::steady_clock::time_point timestamp;
    Exchange exchange;
    EventType event_type;
    double price;
    double quantity;
};