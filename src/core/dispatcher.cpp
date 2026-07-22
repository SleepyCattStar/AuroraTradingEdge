#include "core/dispatcher.hpp"


Dispatcher::Dispatcher(ThreadSafeQueue<MarketEvent>& input_queue)
    :input_queue(input_queue), running(false)
    {

    }

void Dispatcher::addSubscriber(ThreadSafeQueue<MarketEvent>& queue)
{
    // subscribers is the vector holding all the listeners
    subscribers.push_back(&queue);
}

void Dispatcher::run(){
    running = true;
    while(running){
        MarketEvent event;
        if(input_queue.pop(event)){
            for (auto* subscriber : subscribers)
                {
                    // auto* subscriber, each element in subscribers is [ ThreadSafeQueue<MarketEvent>* ]
                    subscriber->push(event);
                }
        }
    }
}

void Dispatcher::stop(){
    running = false;
}