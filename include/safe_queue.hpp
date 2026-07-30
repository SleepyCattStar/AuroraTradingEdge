#pragma once
#include<condition_variable>
#include<mutex>
#include<queue>
#include<atomic>
using namespace std;
template <typename T>

class ThreadSafeQueue{
    private:
        queue<T> q;
        condition_variable cv;
        mutable mutex mtx;  // allows locking in const member functions
        bool finished = false;
        std::atomic<std::size_t> current_size{0};

    public:
        // std::size_t size() const;
        void push(T value){
            {
            lock_guard<mutex> lock(mtx);
            q.push(value);
            current_size.fetch_add(1, std::memory_order_relaxed);
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
            current_size.fetch_sub(1, std::memory_order_relaxed);
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

    std::size_t size() const
        {
            // std::lock_guard<std::mutex> lock(mtx);
            // return q.size();
            // AS THIS is also involving mutex locking, TOO Many locking slows down the engine //
            // So we're implementing it with atomic counter instead of the mutex locking which can be slow
            
            return current_size.load(std::memory_order_relaxed);
        }
};
