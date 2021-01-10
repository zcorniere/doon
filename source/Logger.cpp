#include <Logger.hpp>

Logger::Logger() {}

Logger::~Logger() { this->stop(); }

void Logger::start()
{
    msgT = std::thread([this]() {
        while (!bExit) {
            try {
                qMsg.wait();
                while (!qMsg.empty()) {
                    std::cerr << qMsg.pop_front() << "\e[0m" << std::endl;
                }
            } catch (const std::exception &e) {
                std::cerr << "LOGGER ERROR:" << e.what() << std::endl;
            }
        }
    });
}

void Logger::stop()
{
    bExit = true;
    qMsg.setWaitMode(false);
    if (msgT.joinable()) { msgT.join(); }
}

void Logger::endl()
{
    std::string msg(mBuffers.at(std::this_thread::get_id()).str());
    qMsg.push_back(msg);
    mBuffers.at(std::this_thread::get_id()) = std::stringstream();
}

std::stringstream &Logger::warn(const std::string &msg)
{
    if (!mBuffers.contains(std::this_thread::get_id())) {
        mBuffers.insert({std::this_thread::get_id(), std::stringstream()});
    }
    auto &buf = mBuffers.at(std::this_thread::get_id());
    buf << "\e[33m[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::err(const std::string &msg)
{
    if (!mBuffers.contains(std::this_thread::get_id())) {
        mBuffers.insert({std::this_thread::get_id(), std::stringstream()});
    }
    auto &buf = mBuffers.at(std::this_thread::get_id());
    buf << "\e[31m[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::info(const std::string &msg)
{
    if (!mBuffers.contains(std::this_thread::get_id())) {
        mBuffers.insert({std::this_thread::get_id(), std::stringstream()});
    }
    auto &buf = mBuffers.at(std::this_thread::get_id());
    buf << "[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::debug(const std::string &msg)
{
    if (!mBuffers.contains(std::this_thread::get_id())) {
        mBuffers.insert({std::this_thread::get_id(), std::stringstream()});
    }
    auto &buf = mBuffers.at(std::this_thread::get_id());
    buf << "\e[36m[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::msg(const std::string &msg)
{
    if (!mBuffers.contains(std::this_thread::get_id())) {
        mBuffers.insert({std::this_thread::get_id(), std::stringstream()});
    }
    auto &buf = mBuffers.at(std::this_thread::get_id());
    buf << "[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::raw() {
    if (!mBuffers.contains(std::this_thread::get_id())) {
        mBuffers.insert({std::this_thread::get_id(), std::stringstream()});
    }
    return mBuffers.at(std::this_thread::get_id());
}
