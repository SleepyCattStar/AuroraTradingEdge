#include "monitoring/metrics.hpp"

std::atomic<std::uint64_t> Metrics::messages_received_{0};
std::atomic<std::uint64_t> Metrics::trades_processed_{0};
std::atomic<std::size_t> Metrics::queue_depth_{0};
std::atomic<double> Metrics::average_latency_us_{0.0};
std::atomic<std::uint64_t> Metrics::p99_latency_us_{0};

void Metrics::incrementMessagesReceived()
{
    // ++messages_received_;
    messages_received_.fetch_add(1);

    // Lock-free atomic increment for thread safety
    // IT avoids RACE conditions
}

void Metrics::incrementTradesProcessed(){
    // ++trades_processed_;
    trades_processed_.fetch_add(1);
}

void Metrics::setQueueDepth(std::size_t depth){
    queue_depth_ = depth;
}

void Metrics::setAverageLatency(double latency_us){
    average_latency_us_ = latency_us;
}

void Metrics::setP99Latency(std::uint64_t latency_us){
    p99_latency_us_ = latency_us;
}

std::uint64_t Metrics::messagesReceived()
{
    return messages_received_.load();
}

std::uint64_t Metrics::tradesProcessed()
{
    return trades_processed_.load();
}

std::size_t Metrics::queueDepth()
{
    return queue_depth_.load();
}

double Metrics::averageLatency()
{
    return average_latency_us_.load();
}

std::uint64_t Metrics::p99Latency()
{
    return p99_latency_us_.load();
}
