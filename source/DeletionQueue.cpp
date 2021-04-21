#include "DeletionQueue.hpp"

void DeleteionQueue::push(std::function<void()> &&function)
{
    deletor.push_back(function);
}

void DeleteionQueue::flush()
{
    for (auto it = deletor.rbegin(); it != deletor.rend(); it++) { (*it)(); }
    deletor.clear();
}
