#include "market/market_data_consumer.hpp"
#include "market/market_data_producer.hpp"
#include "analytics/market_analyzer.hpp"
#include "safe_queue.hpp"
#include "core/dispatcher.hpp"


#include<chrono>
#include<thread>
#include<iostream>

int main(void){
    ThreadSafeQueue<MarketEvent> market_queue;
    ThreadSafeQueue<MarketEvent> analysis_queue;

    MarketDataProducer producer(market_queue);

    Dispatcher dispatcher(market_queue,analysis_queue);
    // here the analysis_queue is the output queue, that is accessed by other functions too.

    // market analyser will provide info based on the analysis_queue
    MarketAnalyzer analyser(analysis_queue);

    //passing function pointer and object pointer
    std::thread producer_thread(&MarketDataProducer::run,&producer);

    //implementing dispatcher
    std::cout << "[ENGINE] Starting...\n";
    std::thread dispatcher_thread(&Dispatcher::run, &dispatcher);
    std::thread analyser_thread(&MarketAnalyzer::run, &analyser);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    producer.stop();
    dispatcher.stop();
    analyser.stop();
    market_queue.shutdown();
    analysis_queue.shutdown();

    producer_thread.join();
    dispatcher_thread.join();
    analyser_thread.join();
    std::cout << "[ENGINE] Shutdown complete.\n";

}