#include "core/dispatcher.hpp"


Dispatcher::Dispatcher(ThreadSafeQueue<MarketEvent>& input_queue,
    ThreadSafeQueue<MarketEvent>& output_queue)
    :input_queue(input_queue), output_queue(output_queue), running(false)
    {

    }

void Dispatcher::run(){
    running = true;
    while(running){
        MarketEvent event;
        if(input_queue.pop(event)){
            output_queue.push(event);
        }
    }
}

void Dispatcher::stop(){
    running = false;
}