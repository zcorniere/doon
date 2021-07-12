#pragma once

#include <ThreadedQ.hpp>
#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <type_traits>

class ThreadManager
{
    using WorkUnits = std::shared_ptr<std::packaged_task<void(int)>>;

public:
    ThreadManager();
    ThreadManager(const ThreadManager &) = delete;
    ThreadManager(ThreadManager &) = delete;
    ~ThreadManager();
    void start(const unsigned = std::thread::hardware_concurrency());
    void stop();
    constexpr size_t size() { return thread_p.size(); }
    void resize(const unsigned size);

    template <class F, typename... Args>
    inline auto push(F &&f, Args &&...args) -> std::future<decltype(f(0, args...))>
    {
        static_assert(std::is_invocable<F, int, Args...>(),
                      "The provided function must invocable with provided arguments !");
        auto packagedFunction =
            std::make_shared<std::packaged_task<decltype(f(0, args...))(int)>>(std::bind(
                std::forward<F>(f), std::placeholders::_1, std::forward<Args>(args)...));
        qWork.push_back(packagedFunction);
        return packagedFunction->get_future();
    }

private:
    void new_thread(const unsigned i) noexcept;

private:
    ThreadedQ<WorkUnits> qWork;

    std::atomic_bool bExit = false;
    std::vector<std::thread> thread_p;
};

extern ThreadManager *thread_manager;
