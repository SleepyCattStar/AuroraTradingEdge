#include "logger/market_logger.hpp"
#include "utils/formatter.hpp"
#include<iostream>

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

MarketLogger::MarketLogger(ThreadSafeQueue<MarketEvent>& queue,
                           const std::string& filename)
    : queue(queue),
      running(false)
{
    namespace fs = std::filesystem;
    fs::path log_dir = fs::path(PROJECT_ROOT) / "logs";
    if (!fs::exists(log_dir))
    {
        fs::create_directory(log_dir);
    }

    fs::path log_file = log_dir / filename;

    if (fs::exists(log_file))
    {
        std::string stem = log_file.stem().string();
        std::string extension = log_file.extension().string();
        int counter = 1;
        while (true)
        {
            fs::path candidate =
                log_dir / (stem + "_" + std::to_string(counter) + extension);

            if (!fs::exists(candidate))
            {
                log_file = candidate;
                break;
            }
            counter++;
        }
    }
    file.open(log_file);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open log file.");
    }

    file << "timestamp,symbol,exchange,event_type,price,quantity\n";
}


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
    std::cout << "[LOGGER] Event received\n";
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

