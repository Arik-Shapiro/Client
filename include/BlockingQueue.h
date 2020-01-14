//
// Created by arik on 14.1.2020.
//

#ifndef CLIENT_BLOCKINGQUEUE_H
#define CLIENT_BLOCKINGQUEUE_H


#include <mutex>
#include <condition_variable>
#include <deque>
template <typename T>
class BlockingQueue
{
private:
    std::mutex              d_mutex;
    std::condition_variable d_condition;
    std::deque<T>           d_queue;
public:
    BlockingQueue();
    void push(T const& value);
    T pop();
};

template<typename T>
BlockingQueue<T>::BlockingQueue() = default;

template<typename T>
void BlockingQueue<T>::push(const T &value) {
    {
        std::unique_lock<std::mutex> lock(this->d_mutex);
        d_queue.push_front(value);
    }
    this->d_condition.notify_one();
}

template<typename T>
T BlockingQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(this->d_mutex);
    this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
    std::string rc(std::move(this->d_queue.back()));
    this->d_queue.pop_back();
    return rc;
}

#endif //CLIENT_BLOCKINGQUEUE_H