#pragma once

#include "market/live_market_data_producer.hpp"

// Binance-specific implementation of a live market data producer.
//
// This class establishes a WebSocket connection to Binance,
// receives live market data, converts incoming messages into
// MarketEvents, and publishes them into the shared event queue.

class BinanceMarketDataProducer : public LiveMarketDataProducer
{
public:

    explicit BinanceMarketDataProducer(
        ThreadSafeQueue<MarketEvent>& queue);

    void run() override;
    void stop() override;
};


// using override to override the virtual run() from parent class