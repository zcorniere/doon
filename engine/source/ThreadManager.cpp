#include "ThreadManager.hpp"
#include "Logger.hpp"

ThreadManager::ThreadManager() {}

ThreadManager::~ThreadManager() { this->stop(); }

void ThreadManager::start(const unsigned i) { this->resize(i); }

void ThreadManager::stop()
{
    bExit = true;
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

void ThreadManager::new_thread(const unsigned id)
{
    std::function<void(int)> work;
    logger->info("THREAD_POOL") << "New thread: " << id;
    logger->endl();
    while (1) {
        try {
            std::this_thread::yield();
            if (this->bExit) break;
            {
                const std::scoped_lock lock(q_mut);
                if (this->qWork.empty()) continue;
                work = std::move(this->qWork.front());
                this->qWork.pop();
            }
            if (work) work(id);
        } catch (const std::exception &e) {
            logger->err("THREAD_POOL") << id << " : " << e.what();
            logger->endl();
        } catch (...) {
            logger->err("THREAD_POOL") << "Unkown error on thread " << id;
            logger->endl();
        }
    };
    logger->info("THREAD_POOL") << "End thread: " << id;
    logger->endl();
}
