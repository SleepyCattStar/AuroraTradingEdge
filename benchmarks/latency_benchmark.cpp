#include "analytics/market_analyzer.hpp"
#include "market/binance_market_data_producer.hpp"
#include "market/market_event.hpp"
#include "safe_queue.hpp"

#include <chrono>
#include <iostream>
#include <thread>

constexpr int benchmark_duration_seconds = 60;


int main()
{
    ThreadSafeQueue<MarketEvent> queue;
    MarketAnalyzer analyzer(queue);
    BinanceMarketDataProducer producer(
        queue,
        {"btcusdt"}
    );

    // lambda funcs for the thread
    std::thread producer_thread(
        [&producer]()
        {
            producer.run();
        }
    );
    std::thread analyzer_thread(
        [&analyzer]()
        {
            analyzer.run();
        }
    );

    std::this_thread::sleep_for(
        std::chrono::seconds(benchmark_duration_seconds)
    );

    producer.stop();
    analyzer.stop();

    producer_thread.join();
    analyzer_thread.join();

    LatencyStats stats = analyzer.getLatencyStats();

    double throughput =
        static_cast<double>(stats.event_count) /
        benchmark_duration_seconds;

    std::cout
        << "\n========== LATENCY BENCHMARK ==========\n"
        << "Duration           : "
        << benchmark_duration_seconds << " s\n"
        << "Events Processed   : "
        << stats.event_count << '\n'
        << "Throughput         : "
        << throughput << " events/sec\n"
        << "Average Latency    : "
        << stats.average_latency_us << " us\n"
        << "Minimum Latency    : "
        << stats.min_latency_us << " us\n"
        << "Maximum Latency    : "
        << stats.max_latency_us << " us\n"
        << "========================================\n";

    return 0;
}