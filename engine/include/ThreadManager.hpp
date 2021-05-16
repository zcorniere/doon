#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <type_traits>

class ThreadManager
{
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
        auto packagedFunction =
            std::make_shared<std::packaged_task<decltype(f(0, args...))(int)>>(std::bind(
                std::forward<F>(f), std::placeholders::_1, std::forward<Args>(args)...));
        std::function<void(int id)> storageFunc(
            [packagedFunction](int id) { (*packagedFunction)(id); });
        {
            const std::scoped_lock lock(q_mut);
            qWork.push(storageFunc);
        }
        return packagedFunction->get_future();
    }

private:
    void new_thread(const unsigned i) noexcept;

private:
    std::mutex q_mut;
    std::queue<std::function<void(int)>> qWork;

    std::atomic_bool bExit = false;
    std::deque<std::thread> thread_p;
};

extern ThreadManager *thread_manager;
