// Mock Market Data generation
#pragma once
#include "../safe_queue.hpp"
#include "market_event.hpp"

class MarketDataProducer{
    private:
        ThreadSafeQueue<MarketEvent> &queue;
        bool running = false;    
        MarketEvent generateMarketEvent();
    
    public: 
        MarketDataProducer(ThreadSafeQueue<MarketEvent> &queue);
        void run();
        void stop();
};