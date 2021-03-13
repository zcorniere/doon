#include "Logger.hpp"

Logger logger;

__attribute__((constructor)) void ctor() { logger.start(); }
__attribute__((destructor)) void dtor() { logger.stop(); }