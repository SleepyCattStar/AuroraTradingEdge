#pragma once

#include<fstream>
#include<mutex>
#include<string>
#include<filesystem>
#include<unordered_map>
#include<chrono>
#include "analytics/ohlc.hpp"

class OHLCLogger{

    private:
        // static std::ofstream ohlc_logfile;
        static std::mutex mutex;
        static bool initialized;
        // static void initialize();
        static void initialize(std::chrono::seconds interval);
        static std::string getFilename(std::chrono::seconds interval);

    public:
        // static void logohlcevent(const OHLC &candle)
        // ;
        static void logOHLCEvent(
            const OHLC& candle,
            std::chrono::seconds interval
        );
        static std::unordered_map<
            std::string,
            std::ofstream
        > ohlc_logfiles;

        // this unordered map contains which files are initialised , different time candles different files
};