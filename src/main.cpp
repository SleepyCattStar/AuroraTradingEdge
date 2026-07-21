#include "market/market_data_consumer.hpp"
#include "market/market_data_producer.hpp"
#include "safe_queue.hpp"


#include<chrono>
#include<thread>
#include<iostream>

int main(void){
    ThreadSafeQueue<MarketEvent> queue;

    // objects
    MarketDataProducer producer(queue);
    MarketDataConsumer consumer(queue);
    
    std::cout << "[SYSTEM] Engine Started" << "\n";
    //Launching threads
    std::thread producerThread(&MarketDataProducer::run, &producer);
    std::thread consumerThread(&MarketDataConsumer::run, &consumer);

    //Engine running
    std::this_thread::sleep_for(std::chrono::seconds(10));
    producer.stop();
    consumer.stop();
    queue.shutdown();

    producerThread.join();
    consumerThread.join();

    std::cout << "[ENGINE] Engine Shutdown" << "\n";

}