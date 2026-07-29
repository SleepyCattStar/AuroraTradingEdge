#pragma once

#include <atomic>
#include <cstdint>

class Metrics
{
public:
    static void incrementMessagesReceived();
    static void incrementTradesProcessed();
    static void setQueueDepth(std::size_t depth);
    static void setAverageLatency(double latency_us);
    static void setP99Latency(std::uint64_t latency_us);
    static std::uint64_t messagesReceived();
    static std::uint64_t tradesProcessed();
    static std::size_t queueDepth();
    static double averageLatency();
    static std::uint64_t p99Latency();

private:
    static std::atomic<std::uint64_t> messages_received_;
    static std::atomic<std::uint64_t> trades_processed_;
    static std::atomic<std::size_t> queue_depth_;
    static std::atomic<double> average_latency_us_;
    static std::atomic<std::uint64_t> p99_latency_us_;
};

// implemented atomics for better thread handling