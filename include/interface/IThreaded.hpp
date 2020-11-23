#include <thread>
#include <mutex>
#include <condition_variable>
#include "Snitch.hpp"

#ifndef _ITHREADED_HPP_
#define _ITHREADED_HPP_

class IThreaded {
    public:
        IThreaded() = default;
        IThreaded(const IThreaded &) = delete;
        virtual ~IThreaded() {};
        virtual void run() = 0;
        virtual void stop() {
            this->update();
            if (thread.joinable()) {
                thread.join();
            }
        };
        virtual void update() {
            std::unique_lock<std::mutex> ul(mVariable);
            vBlocking.notify_one();
        }
        virtual void run_threaded() {
            try {
                thread = std::thread([this]() { this->run(); });
            } catch (const std::exception &e) {
                Snitch::err() << e.what() << Snitch::endl;
            }
        }

    protected:
        virtual void wait() {
            std::unique_lock<std::mutex> ul(mVariable);
            vBlocking.wait(ul);
        }

    protected:
        std::thread thread;

        std::mutex mVariable;
        std::condition_variable vBlocking;
};

#endif //_ITHREADED_HPP_
