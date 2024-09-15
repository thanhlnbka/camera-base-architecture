#ifndef LIMITED_QUEUE_HPP_
#define LIMITED_QUEUE_HPP_

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <limits>
#include <queue>
#include <memory> 

template <typename T>
class LimitedQueue {
public:
    LimitedQueue(size_t maxsize) : maxsize(maxsize) {}

    
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex); 
        if (queue.size() == maxsize) {
            queue.pop(); 
        }
        queue.push(value);
    }

    
    bool is_full() const {
        std::lock_guard<std::mutex> lock(mutex); 
        return queue.size() == maxsize;
    }

    
    bool is_empty() const {
        std::lock_guard<std::mutex> lock(mutex); 
        return queue.empty();
    }

    
    void clear() {
        std::lock_guard<std::mutex> lock(mutex); 
        while (!queue.empty()) {
            queue.pop();
        }
    }

    
    T get() const {
        std::lock_guard<std::mutex> lock(mutex); 
        if (!queue.empty()) {
            return queue.front();
        } else {
            return T(); 
        }
    }

    
    void pop() {
        std::lock_guard<std::mutex> lock(mutex); 
        if (!queue.empty()) {
            queue.pop();
        }
    }

    
    size_t get_size() const {
        std::lock_guard<std::mutex> lock(mutex); 
        return queue.size();
    }

    
    LimitedQueue(const LimitedQueue& other) {
        std::lock_guard<std::mutex> lock(other.mutex);
        queue = other.queue;
        maxsize = other.maxsize;
    }

    
    LimitedQueue& operator=(const LimitedQueue& other) {
        if (this != &other) {
            std::lock_guard<std::mutex> lock(mutex);
            std::lock_guard<std::mutex> otherLock(other.mutex);
            queue = other.queue;
            maxsize = other.maxsize;
        }
        return *this;
    }

    
    LimitedQueue clone() const {
        std::lock_guard<std::mutex> lock(mutex);
        LimitedQueue clonedQueue(maxsize);
        std::queue<T> tempQueue = queue; 
        clonedQueue.queue = std::move(tempQueue); 
        return clonedQueue;
    }

private:
    std::queue<T> queue;   
    size_t maxsize;        
    mutable std::mutex mutex; 
};

#endif 
