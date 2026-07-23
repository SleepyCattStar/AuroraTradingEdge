#pragma once
#include "market_event.hpp"
#include "../safe_queue.hpp"

// Abstract base class representing a live market data source.
//
// Every exchange (Binance, Coinbase, Delta, etc.) will inherit from
// this class and implement its own networking and JSON parsing.
//
// The rest of the engine communicates only through MarketEvent,
// so downstream components remain independent of the exchange.

class LiveMarketDataProducer
{
protected:

    // Shared event queue used to publish MarketEvents
    ThreadSafeQueue<MarketEvent>& queue;
    bool running;

public:

    explicit LiveMarketDataProducer(ThreadSafeQueue<MarketEvent>& queue)
        : queue(queue), running(false)
    {
    }

    virtual ~LiveMarketDataProducer() = default;

    // Start receiving live market data
    virtual void run() = 0;

    // Stop receiving market data
    virtual void stop() = 0;
};

// we ARE using PROTECTED HERE, so that the child classes (eg. the different exchanges like binance, coinbase etc)
// can get access to the shared queue to push events, etc.