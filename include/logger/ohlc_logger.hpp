#pragma once

#include<fstream>
#include<mutex>
#include<string>
#include<filesystem>
#include "analytics/ohlc.hpp"

class OHLCLogger{

    private:
        static std::ofstream ohlc_logfile;
        static std::mutex mutex;
        static bool initialized;
        static void initialize();

    public:
        static void logohlcevent(const OHLC &candle);
};