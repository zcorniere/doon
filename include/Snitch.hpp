#ifndef SNITCH_HPP
#define SNITCH_HPP

#include <iostream>
#include <string>

class Snitch {
public:
    static std::ostream &output;
    static std::string endl;
    static std::ostream &warn(const std::string &msg = "WARNING");
    static std::ostream &err(const std::string &msg = "ERROR");
    static std::ostream &info(const std::string &msg = "INFO");
    static std::ostream &debug(const std::string &msg = "DEBUG");
    static std::ostream &msg(const std::string &msg = "MESSAGE");
};

#endif
