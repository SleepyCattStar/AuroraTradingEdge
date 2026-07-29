#include "logger/ohlc_logger.hpp"
#include "utils/formatter.hpp" // for timestamp formatting
#include "logger/engine_logger.hpp"
// std::ofstream OHLCLogger::ohlc_logfile;
std::unordered_map<std::string, std::ofstream>
    OHLCLogger::ohlc_logfiles;
std::mutex OHLCLogger::mutex;
// bool OHLCLogger::initialized = false;

///////////////////////////////////////////////////
// BETTER STRUCTURE TO IMPLEMENT LATER ON //
// BASE LOGGER CLASS AND THEN ALL OTHERS INHERIT FROM IT//
// COMMON FUNCTION WILL BE INITIALISE(file_name) //
///////////////////////////////////////////////////



std::string OHLCLogger::getFilename(
    std::chrono::seconds interval
)
{
    switch (interval.count())
    {
    case 60:
        return "ohlc_1m.csv";

    case 300:
        return "ohlc_5m.csv";

    case 900:
        return "ohlc_15m.csv";

    case 3600:
        return "ohlc_1h.csv";

    default:
        return "ohlc_unknown.csv";
    }
}


// very similar to any other logger
void OHLCLogger::initialize(std::chrono::seconds interval)
{
    // std::string filename;
    std::string filename = getFilename(interval);

    if (ohlc_logfiles.contains(filename))
    {
        return;
    }

    std::filesystem::path log_directory =
        std::filesystem::path(PROJECT_ROOT) / "logs";

    std::filesystem::create_directories(log_directory);

    auto file_path = log_directory / filename;

    ohlc_logfiles[filename].open(
        file_path,
        std::ios::app
    );

    if (!ohlc_logfiles[filename].is_open())
    {
        EngineLogger::error(
        "OHLCLogger: Failed to open " + filename
        );
        throw std::runtime_error("Failed to open " + filename);
    }

    if (std::filesystem::file_size(file_path) == 0)
    {
        ohlc_logfiles[filename]
            << "symbol,"
            << "interval_start,"
            << "open,"
            << "high,"
            << "low,"
            << "close,"
            << "volume,"
            << "trade_count\n";

        ohlc_logfiles[filename].flush();
    }
}

void OHLCLogger::logOHLCEvent(
    const OHLC& candle,
    std::chrono::seconds interval
)
{
    initialize(interval);

    std::lock_guard<std::mutex> lock(mutex);

    std::string filename =
        getFilename(interval);
        
    ohlc_logfiles[filename]
        << candle.symbol << ","
        <<format_timestamp(candle.minute_start) << ","
        << candle.open << ","
        << candle.high << ","
        << candle.low << ","
        << candle.close << ","
        << candle.volume << ","
        << candle.trade_count
        << '\n';

    ohlc_logfiles[filename].flush();
}