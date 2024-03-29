#include "ThreadManager.hpp"
#include "Logger.hpp"

constexpr const char *threadManager = "Thread Manager";

ThreadManager::ThreadManager() {}

ThreadManager::~ThreadManager() { this->stop(); }

void ThreadManager::start(const unsigned i) { this->resize(i); }

void ThreadManager::stop()
{
    bExit = true;
    qWork.setWaitMode(false);
    qWork.notify();
    for (auto &i: thread_p) {
        if (i.joinable()) i.join();
    }
    thread_p.resize(0);
}

void ThreadManager::resize(const unsigned size)
{
    unsigned old_size = thread_p.size();
    thread_p.resize(size);
    for (; old_size < thread_p.size(); old_size++) {
        this->thread_p.at(old_size) =
            std::thread(&ThreadManager::new_thread, this, old_size);
    }
}

void ThreadManager::new_thread(const unsigned id) noexcept
{
    std::optional<WorkUnits> work;

    logger->info(threadManager) << "New thread: " << id;

    while (1) {
        try {
            if (this->bExit) break;
            qWork.wait();
            work = qWork.pop_front();
            if (work) (*(*work))(id);
        } catch (const std::exception &e) {
            logger->err(threadManager) << id << " : " << e.what();

        } catch (...) {
            logger->err(threadManager) << "Unkown error on thread " << id;
        }
    };
    logger->info(threadManager) << "End thread: " << id;
}
