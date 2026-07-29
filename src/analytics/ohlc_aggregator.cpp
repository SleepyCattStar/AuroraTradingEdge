#include "analytics/ohlc_aggregator.hpp"
#include "logger/ohlc_logger.hpp"

OHLCAggregator::OHLCAggregator(std::chrono::seconds interval)
    : interval_(interval)
{
}

// THIS FUNCTION is for getting custom candles, say 5mins 10mins, 6mins, 9mins etc etc
std::chrono::system_clock::time_point
truncate_timestamp(
    std::chrono::system_clock::time_point timestamp,
    std::chrono::seconds interval){
    using namespace std::chrono;
    auto seconds_since_epoch =
        duration_cast<seconds>(timestamp.time_since_epoch());
    auto truncated_seconds =
        seconds(
            (seconds_since_epoch.count() / interval.count())
            * interval.count()
        );
    return system_clock::time_point(truncated_seconds);
    }

void OHLCAggregator::processTrade(const MarketEvent& event){
    auto it = current_candles.find(event.symbol);

    // first trade so no candles yet
    if (it == current_candles.end())
    {
        OHLC candle;
        candle.symbol = event.symbol;
        candle.open = event.price;
        candle.high = event.price;
        candle.low = event.price;
        candle.close = event.price;
        candle.volume = event.quantity;
        candle.trade_count = 1;
        candle.minute_start = truncate_timestamp(event.timestamp,interval_);
        current_candles[event.symbol] = candle;
        return;
    }

    // else we get the max and min for the ohlc
    OHLC& candle = it->second;
    auto current_interval =
        truncate_timestamp(event.timestamp, interval_);

    if (current_interval != candle.minute_start)  // if candle isn't in the same timestamp as the previous interval's one
    {
        finalizeCandle(candle);      
        candle.minute_start = current_interval;
        candle.open = event.price;
        candle.high = event.price;
        candle.low = event.price;
        candle.close = event.price;

        candle.volume = event.quantity;
        candle.trade_count = 1;

        return;
    }
    candle.high = std::max(candle.high, event.price);
    candle.low = std::min(candle.low, event.price);
    candle.close = event.price;
    candle.volume += event.quantity;
    candle.trade_count++;
}

void OHLCAggregator::finalizeCandle(const OHLC& candle){
    OHLCLogger::logOHLCEvent(candle,interval_);
}