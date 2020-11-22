#include "Snitch.hpp"

std::ostream &Snitch::output = std::cerr;

std::string Snitch::endl = "\e[0m\n";

std::ostream &Snitch::warn(const std::string &msg)
{
    Snitch::output << "\e[33m[" << msg << "]: ";
    return Snitch::output;
}

std::ostream &Snitch::err(const std::string &msg)
{
    Snitch::output << "\e[31m[" << msg << "]: ";
    return Snitch::output;
}

std::ostream &Snitch::info(const std::string &msg)
{
    Snitch::output << "[" << msg << "]: ";
    return Snitch::output;
}
std::ostream &Snitch::debug(const std::string &msg)
{
    Snitch::output << "\e[36m[" << msg << "]: ";
    return Snitch::output;
}

std::ostream &Snitch::msg(const std::string &msg)
{
    Snitch::output << "[" << msg << "]: ";
    return Snitch::output;
}
