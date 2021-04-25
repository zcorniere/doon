#pragma once
#include <deque>
#include <functional>

class DeleteionQueue
{
public:
    DeleteionQueue() = default;
    DeleteionQueue(DeleteionQueue &) = delete;
    DeleteionQueue(const DeleteionQueue &) = delete;
    void push(std::function<void()> &&);
    void flush();

public:
    std::deque<std::function<void()>> deletor;
};
