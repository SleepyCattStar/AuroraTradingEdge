#include "include/market/market_data_producer.hpp"
#include "include/safe_queue.hpp"
#include "include/market/market_event.hpp"

#include<random>
#include<chrono>
#include<thread>

MarketDataProducer::MarketDataProducer(ThreadSafeQueue<MarketEvent>& queue) :queue(queue)
{
    running = false;
};

MarketEvent MarketDataProducer::generateMarketEvent(){
    std::string symbol = "BTC";
    Exchange exchange = Exchange::BINANCE;
    EventType event_type = EventType::TRADE;
    auto timestamp = std::chrono::steady_clock::now();

    // rng 
    static std::random_device rd;
    static std::mt19937 gen(rd());
    // static std::uniform_real_distribution<double> distrib(0.78, 1.0);
    static std::uniform_real_distribution<double> price_dist(0.98,1.02);
    static std::uniform_real_distribution<double> quantity_dist(0.01,2.0);
    double price = 76500 * price_dist(gen);
    double quantity = quantity_dist(gen);

    return MarketEvent{
        symbol,
        timestamp,
        exchange,
        event_type,
        price,
        quantity
    };
}

void MarketDataProducer::run(){
    running = true;
    while(running){
        MarketEvent event = generateMarketEvent();
        queue.push(event);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void MarketDataProducer::stop(){
    running = false;
}   