#include "logger/ohlc_logger.hpp"
#include "utils/formatter.hpp" // for timestamp formatting

std::ofstream OHLCLogger::ohlc_logfile;
std::mutex OHLCLogger::mutex;
bool OHLCLogger::initialized = false;

///////////////////////////////////////////////////
// BETTER STRUCTURE TO IMPLEMENT LATER ON //
// BASE LOGGER CLASS AND THEN ALL OTHERS INHERIT FROM IT//
// COMMON FUNCTION WILL BE INITIALISE(file_name) //
///////////////////////////////////////////////////



// very similar to any other logger
void OHLCLogger::initialize(){
    if (initialized)
    {
        return;
    }
    std::filesystem::path log_directory =
        std::filesystem::path(PROJECT_ROOT) / "logs";

    std::filesystem::create_directories(log_directory);

    ohlc_logfile.open(
        log_directory / "ohlc.csv",
        std::ios::app
    );
    if (!ohlc_logfile.is_open())
    {
        throw std::runtime_error("Failed to open ohlc.csv");
    }
    if (std::filesystem::file_size(log_directory / "ohlc.csv") == 0)
    {
        ohlc_logfile
            << "symbol,"
            << "interval_start,"
            << "open,"
            << "high,"
            << "low,"
            << "close,"
            << "volume,"
            << "trade_count\n";

        ohlc_logfile.flush();
    }

    initialized = true;
}


void OHLCLogger::logohlcevent(const OHLC &candle){
    if (!initialized)
    {
        initialize();
    }

    std::lock_guard<std::mutex> lock(mutex);

    //format_timestamp from the utils/formatter.hpp
    ohlc_logfile
        << candle.symbol << ","
        << format_timestamp(candle.minute_start) << ","
        << candle.open << ","
        << candle.high << ","
        << candle.low << ","
        << candle.close << ","
        << candle.volume << ","
        << candle.trade_count
        << '\n';

    ohlc_logfile.flush();
}