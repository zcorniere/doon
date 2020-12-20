#include <ThreadPool.hpp>

ThreadPool::ThreadPool(const unsigned size) { this->resize(size); }

ThreadPool::~ThreadPool() { this->stop(); }

void ThreadPool::stop()
{
    bExit = true;
    for (auto &i: thread_p) {
        if (i.joinable()) i.join();
    }
}

void ThreadPool::resize(const unsigned size) { thread_p.resize(size); }

void ThreadPool::new_thread(const unsigned id)
{
    auto work = [this, id]() {
        while (!this->bExit) {
            try {
                auto work = this->qWork.pop_front();
                work(id);
            } catch (const std::exception &e) {
                Snitch::err("THREAD_POOL") << e.what() << Snitch::endl;
            }
            this->qWork.wait();
        };
    };
    this->thread_p.at(id) = std::thread(work);
}
