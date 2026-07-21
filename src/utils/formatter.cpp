#include "utils/formatter.hpp"
#include <iomanip>
#include <sstream>
#include <ctime>

std::string to_string(Exchange exchange){
    switch(exchange) {
        case Exchange::BINANCE:
            return "BINANCE";
            
        case Exchange::DELTA_EXCHANGE:
            return "DELTA_EXCHANGE";
            
        case Exchange::COINBASE:
            return "COINBASE";
        
        case Exchange::MOCK:
            return "MOCK";
        
        default:
            return "UNKNOWN";
            
    }
}

std::string to_string(EventType event){
    switch(event){
        case EventType::TRADE:
            return "TRADE";
        case EventType::BID_UPDATE:
            return "BID_UPDATE";
        case EventType::ASK_UPDATE:
            return "ASK_UPDATE";
        default:
            return "UNKNOWN";
    }
}

std::string format_timestamp(const std::chrono::system_clock::time_point& tp)
{
    auto time = std::chrono::system_clock::to_time_t(tp);

    std::tm tm = *std::localtime(&time);

    std::ostringstream oss;

    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}