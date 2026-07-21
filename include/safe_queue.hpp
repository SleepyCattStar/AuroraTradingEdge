#pragma once
#include<condition_variable>
#include<mutex>
#include<queue>
using namespace std;
template <typename T>

class ThreadSafeQueue{
    private:
        queue<T> q;
        condition_variable cv;
        mutex mtx;
        bool finished = false;

    public:
        void push(T value){
            {
            lock_guard<mutex> lock(mtx);
            q.push(value);
            }
            cv.notify_one();
        }

        bool pop(T &value){
            unique_lock<mutex> lock(mtx);

            // we wait till queue has data or asked to finish
            cv.wait(lock,[this]{return !q.empty() || finished; });

            if(q.empty() && finished) return false;

            value = q.front();
            q.pop();
            return true;
        }

        // used for releasing the threads, that may have been blocked in the pop() operation
        void shutdown(){
            {
                lock_guard<mutex> lock(mtx);
                finished = true;
            }
            cv.notify_all();
        }
};
