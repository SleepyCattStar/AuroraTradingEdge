#include "market/market_data_consumer.hpp"
#include "market/market_data_producer.hpp"
#include "analytics/market_analyzer.hpp"
#include "safe_queue.hpp"
#include "core/dispatcher.hpp"
#include "logger/market_logger.hpp"
#include "market/binance_market_data_producer.hpp"


#include<chrono>
#include<thread>
#include<iostream>

int main(void){
    ThreadSafeQueue<MarketEvent> market_queue;
    ThreadSafeQueue<MarketEvent> analysis_queue;
    ThreadSafeQueue<MarketEvent> logger_queue;

    //
    // market analyser will provide info based on the analysis_queue
    MarketAnalyzer analyser(analysis_queue);
    MarketLogger logger(logger_queue, "market_data.csv");
    // MarketDataProducer producer(market_queue);
    BinanceMarketDataProducer producer(market_queue);
    Dispatcher dispatcher(market_queue);
    
    ///////////////////////////////////
    // SUBSCRIBERS TO THE DISPATCHER //
    //////////////////////////////////
    dispatcher.addSubscriber(logger_queue);
    dispatcher.addSubscriber(analysis_queue);
    // here the analysis_queue is the output queue, that is accessed by other functions too.



    //passing function pointer and object pointer
    // std::thread producer_thread(&MarketDataProducer::run,&producer);
    std::thread producer_thread(&BinanceMarketDataProducer::run,&producer);

    //////////////////////////////////////////////
    // BETTER APPROACH IS USING LAMBDA function //
    // std::thread producer_thread([&]()
    //     {
    //         producer.run();
    //     });
    //////////////////////////////////////////////


    //implementing dispatcher
    std::cout << "[ENGINE] Starting...\n";
    std::thread dispatcher_thread(&Dispatcher::run, &dispatcher);
    std::thread analyser_thread(&MarketAnalyzer::run, &analyser);
    std::thread logger_thread(&MarketLogger::run, &logger);

    std::this_thread::sleep_for(std::chrono::seconds(10));


    // STOPPING THE CREATED OBJECTS
    producer.stop();
    dispatcher.stop();
    analyser.stop();
    logger.stop();

    // SHUTTING DOWN THE QUEUES
    market_queue.shutdown();
    analysis_queue.shutdown();
    logger_queue.shutdown();

    //////////////////////////////
    /// JOINING THREADS TO MAIN //
    //////////////////////////////
    producer_thread.join();
    dispatcher_thread.join();
    analyser_thread.join();
    logger_thread.join();
    std::cout << "[ENGINE] Shutdown complete.\n";

}