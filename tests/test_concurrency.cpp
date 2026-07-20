#include<iostream>
#include<atomic>
#include<queue>
#include "../include/safe_queue.hpp"
#include "../include/data_types.hpp"
#include<thread>

using namespace std;

const int test_candles = 1e6; 

ThreadSafeQueue<ohlc> q;
atomic<long long> count{0};


void producer(){
    for(int i =0; i<test_candles; i++){
        q.push({})
    }
}