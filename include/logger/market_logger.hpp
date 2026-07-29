#pragma once

#include<string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include<chrono>
#include <stdexcept>
#include "safe_queue.hpp"
#include "market/market_event.hpp"


class MarketLogger{
    private:
        ThreadSafeQueue<MarketEvent>& queue;
        bool running;
        std::ofstream file;
        void logEvent(const MarketEvent& event);

    public:
        MarketLogger(ThreadSafeQueue<MarketEvent>& queue,
             const std::string& filename);
        void run();
        void stop();
};

// Dispatcher --> logger_queue --> marketlogger -> market_data.csv