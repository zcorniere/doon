#include "Snitch.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>

#ifndef _ATHREADED_HPP_
#define _ATHREADED_HPP_

class AThreaded
{
public:
    AThreaded() = default;
    AThreaded(const AThreaded &) = delete;
    virtual ~AThreaded(){};
    virtual void run() = 0;
    virtual void stop()
    {
        this->update();
        if (thread.joinable()) { thread.join(); }
    };
    virtual void update()
    {
        std::unique_lock<std::mutex> ul(mVariable);
        vBlocking.notify_one();
    }
    virtual void run_threaded()
    {
        try {
            thread = std::thread([this]() { this->run(); });
        } catch (const std::exception &e) {
            Snitch::err() << e.what() << Snitch::endl;
        }
    }

protected:
    virtual void wait()
    {
        std::unique_lock<std::mutex> ul(mVariable);
        vBlocking.wait(ul);
    }

protected:
    std::thread thread;

    std::mutex mVariable;
    std::condition_variable vBlocking;
};

#endif    //_ATHREADED_HPP_
