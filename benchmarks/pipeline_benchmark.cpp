// Functional Benchmark (ESSENTIALLY VERIFYING THE WORKING OF OUR PIPELINE)

#include "market/market_data_producer.hpp"
#include "analytics/market_analyzer.hpp"
#include "analytics/analyzer_stats.hpp"
#include "core/dispatcher.hpp"
#include "safe_queue.hpp"

#include <chrono>
#include <thread>
#include <iostream>

constexpr int BENCHMARK_TIME = 10; // seconds

int main(){
    ThreadSafeQueue<MarketEvent> market_queue;
    ThreadSafeQueue<MarketEvent> analysis_queue;

    MarketDataProducer producer(market_queue);
    Dispatcher dispatcher(market_queue, analysis_queue);
    MarketAnalyzer analyzer(analysis_queue);

    auto start = std::chrono::steady_clock::now();
    std::thread producer_thread(&MarketDataProducer::run, &producer);
    std::thread dispatcher_thread(&Dispatcher::run, &dispatcher);
    std::thread analyzer_thread(&MarketAnalyzer::run, &analyzer);

    std::this_thread::sleep_for(std::chrono::seconds(BENCHMARK_TIME));

    producer.stop();
    dispatcher.stop();
    analyzer.stop();
    market_queue.shutdown();
    analysis_queue.shutdown();

    producer_thread.join();
    dispatcher_thread.join();
    analyzer_thread.join();

    AnalyzerStats stats = analyzer.getStats();
    // trade_count, total_volume, total_trade_value, vwap
    auto stop = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stop - start
    );

    double seconds = duration.count() / 1'000'000.0;
    double throughput = stats.trade_count / seconds;

    std::cout << "=================================\n";
    std::cout << " Pipeline Benchmark\n";
    std::cout << "=================================\n";

    std::cout << "Runtime          : "<< seconds<< " seconds\n";
    std::cout << "Trades processed : "<< stats.trade_count<< "\n";
    std::cout << "Throughput       : "<< throughput<< " trades/sec\n";
    std::cout << "Total Volume     : "<< stats.total_volume<< "\n";
    std::cout << "VWAP             : "<< stats.vwap<< "\n";
    std::cout << "=================================\n\n";
}