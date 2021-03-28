#pragma once

#include "ThreadedQ.hpp"
#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <type_traits>

class ThreadManager
{
public:
    ThreadManager();
    ThreadManager(const ThreadManager &) = delete;
    ThreadManager(ThreadManager &) = delete;
    ~ThreadManager();
    void start();
    void stop();
    constexpr size_t size() { return thread_p.size(); }
    void resize(const unsigned size);

    template <class F, typename... Args>
    inline auto push(F &&f, Args &&...args) -> std::future<decltype(f(0, args...))>
    {
        auto packagedFunction =
            std::make_shared<std::packaged_task<decltype(f(0, args...))(int)>>(std::bind(
                std::forward<F>(f), std::placeholders::_1, std::forward<Args>(args)...));
        std::function<void(int id)> storageFunc(
            [packagedFunction](int id) { (*packagedFunction)(id); });
        qWork.push_back(storageFunc);
        return packagedFunction->get_future();
    }

private:
    void new_thread(const unsigned i);

private:
    ThreadedQ<std::function<void(int)>> qWork;
    std::atomic_bool bExit = false;
    std::deque<std::thread> thread_p;
};

extern ThreadManager *thread_manager;
