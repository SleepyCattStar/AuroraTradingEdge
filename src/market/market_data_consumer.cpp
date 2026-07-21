#include "market/market_data_consumer.hpp"      //class MarketDataConsumer
#include "utils/formatter.hpp"
#include<iostream>


// constructor
MarketDataConsumer::MarketDataConsumer(ThreadSafeQueue<MarketEvent>& queue) 
:queue(queue)
{
    running = false;
};

void MarketDataConsumer::processEvent(const MarketEvent& event){
    std::cout << "Symbol :"<<event.symbol<<"\n";
    // std::cout << "Timestamp :" << event.timestamp<<"\n";
    // auto time = event.timestamp.time_since_epoch();
    // std::cout << "Timestamp: "<<time.count() << "\n";
    std::cout << "Timestamp : " <<format_timestamp(event.timestamp) << "\n";
    // std::cout << "Exchange : " << static_cast<int>(event.exchange) << "\n";
    std::cout << "Exchange : " <<to_string(event.exchange) << "\n";
    // std::cout << "Event :" << static_cast<int>(event.event_type) << "\n";
    std::cout << "Event :" << to_string(event.event_type) << "\n";
    std::cout << "Price :" << event.price << "\n";
    std::cout << "Quantity: "<<event.quantity << "\n";

}

void MarketDataConsumer::run(){
    running = true;

    // note, this is not the standard queue.
    // It is the thread safe queue created by us
    // File: safe_queue.hpp
    while(running){
        MarketEvent event;
        // event = queue.front();
        if(queue.pop(event)){
            processEvent(event);
        }
    }
}

void MarketDataConsumer::stop(){

    running = false;
}