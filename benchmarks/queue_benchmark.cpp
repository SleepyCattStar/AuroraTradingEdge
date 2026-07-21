#include "safe_queue.hpp"
#include "market/market_event.hpp"
#include<chrono>
#include<thread>
#include<iostream>

constexpr int NUM_EVENTS = 1'000'000;

int main(){
    ThreadSafeQueue<MarketData> queue;
}