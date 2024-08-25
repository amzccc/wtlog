#include "wtlog.hpp"
#include <iostream>

using namespace wtlog;
using std::shared_ptr;
using std::cout;
using std::cerr;
using std::endl;

wtlog::Logger::Logger(Logger&& logger) noexcept {
    this->m_impl.swap(logger.m_impl);
}

Logger& wtlog::Logger::operator=(Logger&& logger) noexcept {
    this->m_impl.swap(logger.m_impl);
    return *this;
}

std::shared_ptr<Logger> wtlog::Logger::instance() {
    static std::shared_ptr<Logger> instance(new Logger);
    return instance;
}

Logger& wtlog::Logger::instanceRef() {
    return *instance();
}
