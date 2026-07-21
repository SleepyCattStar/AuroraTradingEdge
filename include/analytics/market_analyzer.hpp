#pragma once
#include "safe_queue.hpp"
#include "market/market_event.hpp"

class MarketAnalyzer{
    private:
        bool running;
        ThreadSafeQueue<MarketEvent> & queue;   
        double total_volume;
        double total_trade_value;
        int trade_count;
    
    public:
        MarketAnalyzer(ThreadSafeQueue<MarketEvent>& queue);
        void processEvent(const MarketEvent& event);
        void run();
        void stop();
};