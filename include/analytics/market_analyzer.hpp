#pragma once
#include "safe_queue.hpp"
#include "market/market_event.hpp"
#include "analytics/analyzer_stats.hpp"
#include<vector>
#include<limits>

class MarketAnalyzer{
    private:
        bool running;
        ThreadSafeQueue<MarketEvent> & queue;   
        double total_volume;
        double total_trade_value;
        int trade_count;
        std::vector<std::uint64_t> latency_samples;
        std::uint64_t total_latency_us = 0;
        std::uint64_t event_count = 0;
        std::uint64_t min_latency_us =
        std::numeric_limits<std::uint64_t>::max();
        std::uint64_t max_latency_us = 0;
    
    public:
        
        AnalyzerStats getStats() const;   // getter
        void record_latency(std::uint64_t latency_us);
        void print_latency_statistics();
        MarketAnalyzer(ThreadSafeQueue<MarketEvent>& queue);
        void processEvent(const MarketEvent& event);
        void run();
        void stop();
};