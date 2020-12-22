#include <condition_variable>
#include <deque>
#include <mutex>

#ifndef _THREADEDQ_HPP_
#define _THREADEDQ_HPP_

#include "Snitch.hpp"

template <typename T>
class ThreadedQ
{
public:
    ThreadedQ() = default;
    ThreadedQ(const size_t size): q(size) {}
    ThreadedQ(const ThreadedQ<T> &) = delete;
    virtual ~ThreadedQ() { this->clear(); }

    const T &front()
    {
        if (this->size() == 0) { throw std::runtime_error("queue is empty"); }
        std::scoped_lock lock(q_mut);
        return q.front();
    }
    const T &back()
    {
        if (this->size() == 0) { throw std::runtime_error("queue is empty"); }
        std::scoped_lock lock(q_mut);
        return q.back();
    }
    bool empty()
    {
        std::scoped_lock lock(q_mut);
        return q.empty();
    }
    size_t size()
    {
        std::scoped_lock lock(q_mut);
        return q.size();
    }
    void clear()
    {
        std::scoped_lock lock(q_mut);
        q.clear();
    }
    T pop_front()
    {
        if (this->size() == 0) { throw std::runtime_error("queue is empty"); }
        std::scoped_lock lock(q_mut);
        T t = std::move(q.front());
        q.pop_front();
        return t;
    }
    T pop_back()
    {
        if (this->size() == 0) { throw std::runtime_error("queue is empty"); }
        std::scoped_lock lock(q_mut);
        T t = std::move(q.front());
        q.pop_back();
        return t;
    }
    void push_back(const T &i)
    {
        std::scoped_lock lock(q_mut);
        q.emplace_back(std::move(i));

        std::unique_lock<std::mutex> ul(mutBlocking);
        vBlocking.notify_one();
    }
    void push_front(const T &i)
    {
        std::scoped_lock lock(q_mut);
        q.emplace_front(std::move(i));

        std::unique_lock<std::mutex> ul(mutBlocking);
        vBlocking.notify_one();
    }

    void wait()
    {
        while (this->empty()) {
            std::unique_lock<std::mutex> ul(mutBlocking);
            vBlocking.wait(ul);
        }
    }
    void notify() { vBlocking.notify_all(); }

private:
    std::mutex q_mut;
    std::deque<T> q;

    std::mutex mutBlocking;
    std::condition_variable vBlocking;
};
#endif    //_THREADEDQ_HPP_
