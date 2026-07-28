#include "analytics/market_analyzer.hpp"
#include "logger/engine_logger.hpp"
#include "logger/analyzer_logger.hpp"
#include "analytics/ohlc_aggregator.hpp"
#include<iostream>
#include <algorithm>


OHLCAggregator ohlc_aggregator{std::chrono::minutes(1)};

MarketAnalyzer::MarketAnalyzer(ThreadSafeQueue<MarketEvent>& queue)
    : queue(queue),
      running(false),
      total_volume(0.0),
      total_trade_value(0.0),
      trade_count(0)
{
    latency_samples.reserve(100000);
}

uint64_t MarketAnalyzer::calculate_percentile(double percentile) const{
        if (latency_samples.empty())
    {
        return 0;
    }

    std::vector<std::uint64_t> sorted_samples =
        latency_samples;

    std::sort(
        sorted_samples.begin(),
        sorted_samples.end()
    );

    std::size_t index =
        static_cast<std::size_t>(
            percentile * (sorted_samples.size() - 1)
        );

    return sorted_samples[index];
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
    ohlc_aggregator.processTrade(event);
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


// to get the values so we can use it for our benchmark
LatencyStats MarketAnalyzer::getLatencyStats() const
{
    LatencyStats stats;

    stats.event_count = event_count;

    if (event_count == 0)
    {
        stats.average_latency_us = 0.0;
        stats.min_latency_us = 0;
        stats.max_latency_us = 0;
        return stats;
    }

    stats.average_latency_us =
        static_cast<double>(total_latency_us) / event_count;

    stats.min_latency_us = min_latency_us;
    stats.max_latency_us = max_latency_us;
    stats.p50_latency_us =
    calculate_percentile(0.50);
    stats.p95_latency_us =
        calculate_percentile(0.95);
    stats.p99_latency_us =
        calculate_percentile(0.99);

    return stats;
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
    
    LatencyStats stats = getLatencyStats();

    try{
        AnalyticsLogger::logLatencyBenchmark(
            stats.event_count,
            stats.average_latency_us,
            stats.min_latency_us,
            stats.max_latency_us,
            stats.p50_latency_us,
            stats.p95_latency_us,
            stats.p99_latency_us
        );
    }
    catch (const std::exception& e){
        EngineLogger::error(std::string("Failed to write to analytics.csv") + e.what());
        std::cout << "[LOGGER] Unknown Error"<<"\n";
    }
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


