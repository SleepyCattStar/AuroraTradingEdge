#include "logger/analyzer_logger.hpp"
#include<filesystem>
#include<utils/formatter.hpp>
#include<logger/engine_logger.hpp>

std::ofstream AnalyticsLogger::log_file;
std::mutex AnalyticsLogger::mutex;
bool AnalyticsLogger::initialized = false;

// VERY SIMILAR TO THE ENGINE_LOGGER.CPP
void AnalyticsLogger::initialize(){
    if (initialized)
    {
        return;
    }
    std::filesystem::path log_directory =
        std::filesystem::path(PROJECT_ROOT) / "logs";

    // path now is trade-engine/logs , here as trade-engine is the project root

    // creates directory logs/ if it doesnt exist
    std::filesystem::create_directories(log_directory);

    log_file.open(
        log_directory / "analytics.csv",
        std::ios::app
    );
    if (!log_file.is_open())
    {
        throw std::runtime_error("Failed to open analytics.csv");
    }
    initialized = true;
}

void AnalyticsLogger::logLatencyBenchmark(std::uint64_t events,
double average_latency, std::uint64_t minimum_latency, std::uint64_t maximum_latency){

    try
    {
        initialize();
    }
    catch (const std::exception& e)
    {
        EngineLogger::error(
            std::string("Failed to initialize AnalyticsLogger: ") +
            e.what()
        );
        return;
    }

    std::lock_guard<std::mutex> lock(mutex);

    log_file
        << events << ","
        << average_latency << ","
        << minimum_latency << ","
        << maximum_latency
        << "\n";

    log_file.flush();

}

