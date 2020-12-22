#include <ThreadPool.hpp>

ThreadPool::ThreadPool(const unsigned size) { this->resize(size); }

ThreadPool::~ThreadPool() { this->stop(); }

void ThreadPool::stop()
{
    bExit = true;
    qWork.notify();
    for (auto &i: thread_p) {
        if (i.joinable()) i.join();
    }
}

void ThreadPool::resize(const unsigned size)
{
    unsigned old_size = thread_p.size();
    thread_p.resize(size);
    for (; old_size < thread_p.size(); old_size++) { this->new_thread(old_size); }
}

void ThreadPool::new_thread(const unsigned id)
{
    auto work = [this, id]() {
        Snitch::info("THREAD_POOL") << "New thread: " << id << Snitch::endl;
        while (!this->bExit) {
            this->qWork.wait();
            try {
                auto work = this->qWork.pop_front();
                work(id);
            } catch (const std::exception &e) {
                Snitch::err("THREAD_POOL") << e.what() << Snitch::endl;
            }
        };
        Snitch::info("THREAD_POOL") << "End thread: " << id << Snitch::endl;
    };
    this->thread_p.at(id) = std::thread(work);
}
