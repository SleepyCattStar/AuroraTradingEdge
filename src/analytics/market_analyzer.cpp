#include "analytics/market_analyzer.hpp"
#include "logger/engine_logger.hpp"
#include "logger/analyzer_logger.hpp"
#include<iostream>

MarketAnalyzer::MarketAnalyzer(ThreadSafeQueue<MarketEvent>& queue)
    : queue(queue),
      running(false),
      total_volume(0.0),
      total_trade_value(0.0),
      trade_count(0)
{
    latency_samples.reserve(100000);
}

void MarketAnalyzer::processEvent(const MarketEvent& event){
    if(event.event_type!= EventType::TRADE){
        return;
    }

    //implementing VMAP, Essentially a weighted average of price , volume
    total_volume+= event.quantity;
    total_trade_value+= event.quantity * event.price;
    trade_count++;

    if(trade_count% 10 == 0){
        AnalyzerStats stats = getStats();
        std::cout << "=========================\n";
        std::cout << "Trades : " << stats.trade_count << "\n";
        std::cout << "Volume : " << stats.total_volume << "\n";
        std::cout << "VWAP   : " << stats.vwap << "\n";
        std::cout << "=========================\n";
    }
}

void MarketAnalyzer::record_latency(std::uint64_t latency_us){
    event_count++;
    if (event_count % 20 == 0)   // using 100 for testing
    {
        print_latency_statistics();
    }
    total_latency_us += latency_us;
    latency_samples.push_back(latency_us);
    min_latency_us = std::min(latency_us, min_latency_us);
    max_latency_us = std::max(latency_us, max_latency_us);
}

void MarketAnalyzer::print_latency_statistics()
{
    if (event_count == 0)
    {
        return;
    }

    double average =
        static_cast<double>(total_latency_us) / event_count;

    std::cout
        << "\n========== LATENCY BENCHMARK ==========\n"
        << "Events Processed : " << event_count << '\n'
        << "Average Latency  : " << average << " us\n"
        << "Minimum Latency  : " << min_latency_us << " us\n"
        << "Maximum Latency  : " << max_latency_us << " us\n"
        << "=======================================\n";

        /*
    EngineLogger::info(
        "Latency Benchmark | Events: " +
        std::to_string(event_count) +
        " Avg: " + std::to_string(average) +
        " us Min: " + std::to_string(min_latency_us) +
        " us Max: " + std::to_string(max_latency_us) +
        " us"
    );

    */
    AnalyticsLogger::logLatencyBenchmark(
        event_count,
        average,
        min_latency_us,
        max_latency_us
    );
}

AnalyzerStats MarketAnalyzer::getStats() const{
    AnalyzerStats stats;

    stats.trade_count = trade_count;
    stats.total_volume = total_volume;
    stats.total_trade_value = total_trade_value;

    if (total_volume == 0)
        stats.vwap = 0.0;
    else
        stats.vwap = total_trade_value / total_volume;

    return stats;
}

void MarketAnalyzer::run(){
    running = true;
    while(running){
        MarketEvent event;
        if(queue.pop(event)){
            auto analyzer_receive_time = std::chrono::steady_clock::now();
            processEvent(event);

        auto latency =
            std::chrono::duration_cast<std::chrono::microseconds>(
                analyzer_receive_time - event.engine_receive_time
            );

        record_latency(latency.count());

        // Optional for debugging
        std::cout << "Engine latency: " << latency.count() << " us\n";
        }
    }
    /*
    if (trade_count % 10 == 0)
        {
            double vwap = total_trade_value / total_volume;
            // printing the vwap
            std::cout << "=========================\n";
            std::cout << "Trades : " << trade_count << "\n";
            std::cout << "Volume : " << total_volume << "\n";
            std::cout << "VWAP   : " << vwap << "\n";
            std::cout << "=========================\n";
        }
    */
    
}

void MarketAnalyzer::stop(){
    std::cout << "Event count :" <<event_count << "\n";
    running = false;
}


