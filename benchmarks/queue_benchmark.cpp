#include "safe_queue.hpp"
#include "market/market_event.hpp"
#include<chrono>
#include<thread>
#include<iostream>

// Note that this is the benchmark just for the execution speed of the queue, we've not implemented the rng for the prices and quantity, as we're just testing the speed of the data-structure


// 1 million events
constexpr int NUM_EVENTS = 1'000'000;

int main(){
    ThreadSafeQueue<MarketEvent> queue;
    auto start = std::chrono::steady_clock::now();
    // lambda function for producer thread
    std::thread producer([&queue](){
        for(int i = 0; i < NUM_EVENTS; i++)
        {
            MarketEvent event;

            event.symbol = "BTC";
            event.exchange = Exchange::BINANCE;
            event.event_type = EventType::TRADE;
            event.price = 75000;
            event.quantity = 1.0;
            event.timestamp = std::chrono::system_clock::now();

            queue.push(event);
        }
    });
    // lambda function for this thread
    std::thread consumer([&queue](){
        MarketEvent event;
        int count = 0;
        while(count < NUM_EVENTS)
        {
            if(queue.pop(event))
            {
                count++;
            }
        }
    });

    producer.join();
    consumer.join();


    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end - start
    );

    // std::cout << "Time Taken: "<<duration.count()<<"us\n";
    double seconds = duration.count() / 1'000'000.0;
    double throughput = NUM_EVENTS / seconds;

    std::cout << "=================================\n";
    std::cout << " ThreadSafe Queue Benchmark\n";
    std::cout << "=================================\n\n";

    std::cout << "Events processed : "<< NUM_EVENTS << "\n";
    std::cout << "Total time       : "<< duration.count() << " microseconds\n";
    std::cout << "Throughput       : "<< throughput<< " events/sec\n";
    std::cout << "=================================\n";

}