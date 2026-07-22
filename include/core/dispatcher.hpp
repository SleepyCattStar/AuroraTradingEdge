#pragma once
#include<vector>
#include "safe_queue.hpp"
#include "market/market_event.hpp"

class Dispatcher{
    private:
        ThreadSafeQueue<MarketEvent>& input_queue;
        // ThreadSafeQueue<MarketEvent>& output_queue;

        /*
           DISPATCHER ARCHITECTURE CHANGE (v2)
           We are implementing fanout architecture so 1 publisher can publish and many subscribers and use that information.
           (Pub Sub reference from ROS2)
        */
        std::vector<ThreadSafeQueue<MarketEvent>*> subscribers;
        bool running;

    public:

        // Dispatcher(ThreadSafeQueue<MarketEvent>& input_queue,
        //     ThreadSafeQueue<MarketEvent>& output_queue);

        // new constructor starts with 0 subscribers
        Dispatcher(ThreadSafeQueue<MarketEvent>& input_queue);

        void addSubscriber(ThreadSafeQueue<MarketEvent>& queue);

        void run();
        void stop();
};