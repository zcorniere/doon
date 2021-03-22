#include "ThreadManager.hpp"
#include "Logger.hpp"

ThreadManager::ThreadManager() {}

ThreadManager::~ThreadManager() { this->stop(); }

void ThreadManager::start() { this->resize(std::thread::hardware_concurrency()); }

void ThreadManager::stop()
{
    bExit = true;
    qWork.setWaitMode(false);
    for (auto &i: thread_p) {
        if (i.joinable()) i.join();
    }
    thread_p.resize(0);
}

void ThreadManager::resize(const unsigned size)
{
    unsigned old_size = thread_p.size();
    thread_p.resize(size);
    for (; old_size < thread_p.size(); old_size++) { this->new_thread(old_size); }
}

void ThreadManager::new_thread(const unsigned id)
{
    this->thread_p.at(id) = std::thread([this, id]() {
        std::optional<std::function<void(int)>> work;
        logger->info("THREAD_POOL") << "New thread: " << id;
        logger->endl();
        while (1) {
            this->qWork.wait();
            if (this->bExit) break;
            try {
                work = this->qWork.pop_front();
                if (work && work.has_value()) (*work)(id);
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
    });
}
