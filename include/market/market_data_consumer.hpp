#pragma once
#include "market_event.hpp"
#include "../safe_queue.hpp"

class MarketDataConsumer{
    private:
        bool running = false;
        ThreadSafeQueue<MarketEvent> &queue;
        void processEvent(const MarketEvent& event);
    
    public: 
        MarketDataConsumer(ThreadSafeQueue<MarketEvent>& queue);
        void run();
        void stop();
};