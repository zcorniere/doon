#include "ThreadManager.hpp"
#include "Logger.hpp"

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

    LOGGER_INFO << "New thread: " << id;
    logger->endl();
    while (1) {
        try {
            if (this->bExit) break;
            qWork.wait();
            work = qWork.pop_front();
            if (work) (*(*work))(id);
        } catch (const std::exception &e) {
            LOGGER_ERR << id << " : " << e.what();
            logger->endl();
        } catch (...) {
            LOGGER_ERR << "Unkown error on thread " << id;
            logger->endl();
        }
    };
    LOGGER_INFO << "End thread: " << id;
    logger->endl();
}
