#include "logger/market_logger.hpp"
#include "utils/formatter.hpp"


namespace fs = std::filesystem;

/*
// initialising the private members of the class inside the constructor
MarketLogger::MarketLogger(ThreadSafeQueue<MarketEvent>& queue,
    const std::string& filename): queue(queue), running(false), file(filename){
        // file << "timestamp,symbol,exchange,event_type,price,quantity\n";
    if (file.is_open())
        {
            file << "timestamp,symbol,exchange,event_type,price,quantity\n";
        }
};
*/


void MarketLogger::logEvent(const MarketEvent& event)
    {
        file
            << format_timestamp(event.timestamp) << ","
            << event.symbol << ","
            << to_string(event.exchange) << ","
            << to_string(event.event_type) << ","
            << event.price << ","
            << event.quantity
            << "\n";
    }


void MarketLogger::run(){

    running = true;
    while(running){
        MarketEvent event;
        if(queue.pop(event))
        {
            logEvent(event);
        }
    }
}

void MarketLogger::stop(){
    running = false;
    if (file.is_open())
        {
            file.close();
        }
}

