#include "analytics/market_analyzer.hpp"
#include<iostream>

MarketAnalyzer::MarketAnalyzer(ThreadSafeQueue<MarketEvent>& queue)
    : queue(queue),
      running(false),
      total_volume(0.0),
      total_trade_value(0.0),
      trade_count(0)
{

}

void MarketAnalyzer::processEvent(const MarketEvent& event){
    if(event.event_type!= EventType::TRADE){
        return;
    }

    //implementing VMAP, Essentially a weighted average of price , volume
    total_volume+= event.quantity;
    total_trade_value+= event.quantity * event.price;
    trade_count++;

    if(trade_count% 10 == 0){
        AnalyzerStats stats = getStats();
        std::cout << "=========================\n";
        std::cout << "Trades : " << stats.trade_count << "\n";
        std::cout << "Volume : " << stats.total_volume << "\n";
        std::cout << "VWAP   : " << stats.vwap << "\n";
        std::cout << "=========================\n";
    }
}

AnalyzerStats MarketAnalyzer::getStats() const{
    AnalyzerStats stats;

    stats.trade_count = trade_count;
    stats.total_volume = total_volume;
    stats.total_trade_value = total_trade_value;

    if (total_volume == 0)
        stats.vwap = 0.0;
    else
        stats.vwap = total_trade_value / total_volume;

    return stats;
}

void MarketAnalyzer::run(){
    running = true;
    while(running){
        MarketEvent event;
        if(queue.pop(event)){
            processEvent(event);
        }
    }
    /*
    if (trade_count % 10 == 0)
        {
            double vwap = total_trade_value / total_volume;
            // printing the vwap
            std::cout << "=========================\n";
            std::cout << "Trades : " << trade_count << "\n";
            std::cout << "Volume : " << total_volume << "\n";
            std::cout << "VWAP   : " << vwap << "\n";
            std::cout << "=========================\n";
        }
    */
    
}

void MarketAnalyzer::stop(){
    running = false;
}


