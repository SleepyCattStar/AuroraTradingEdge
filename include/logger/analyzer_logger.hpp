#pragma once

#include <fstream>
#include <mutex>
#include <string>

class AnalyticsLogger
{
private:
// WE USE STATIC, so theres only 1 instance for any number of analytics logger uses across this project
    static std::ofstream log_file;
    static std::mutex mutex;
    static bool initialized;
    static void initialize();

public:
    static void logLatencyBenchmark(
    std::uint64_t events,
    double average_latency,
    std::uint64_t minimum_latency,
    std::uint64_t maximum_latency,
    std::uint64_t p50_latency,
    std::uint64_t p95_latency,
    std::uint64_t p99_latency
    );
};