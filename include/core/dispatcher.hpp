#pragma once
#include "safe_queue.hpp"
#include "market/market_event.hpp"

class Dispatcher{
    private:
        ThreadSafeQueue<MarketEvent>& input_queue;
        ThreadSafeQueue<MarketEvent>& output_queue;
        bool running;

    public:

        Dispatcher(ThreadSafeQueue<MarketEvent>& input_queue,
            ThreadSafeQueue<MarketEvent>& output_queue);


        void run();
        void stop();
};