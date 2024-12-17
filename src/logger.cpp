#include <wtlog/logger.h>
#include <wtlog/sinks/sink_distributor.h>
#include <map>
#include <algorithm>

using namespace wtlog;
using wtlog::LogGenerator;

std::map<LogLevel, std::string> Logger::m_logflags{
    { LogLevel::fatal, " [F] " },
    { LogLevel::error, " [E] " },
    { LogLevel::warn,  " [W] " },
    { LogLevel::info,  " [I] " },
    { LogLevel::debug, " [D] " },
    { LogLevel::trace, " [T] " },
};

wtlog::Logger::~Logger() {
    if(!m_carrier->empty()) {
        m_distributor->distribute(m_carrier);
    }
}

LogGenerator& wtlog::LogGenerator::instance() {
    static LogGenerator factory;
    return factory;
}

bool wtlog::LogGenerator::remove(const Pointer<Logger>& logger) {
    auto iter = std::remove(m_loggers.begin(), m_loggers.end(), logger);
    if(iter == m_loggers.end()) {
        return false;
    }
    m_loggers.erase(iter, m_loggers.end());
    return true;
}

void wtlog::Logger::attachSinker(Pointer<sinks::Sinker> sinker) {
    m_distributor->registerSink(sinker);
}

void wtlog::Logger::detachSinker(Pointer<sinks::Sinker> sinker) {
    m_distributor->unregisterSink(sinker);
}

void wtlog::Logger::setTimePrecision(utils::Clock::Unit precision) {
    m_clock.setPrecision(precision);
}

void wtlog::Logger::setLevel(const LogLevel level) {
    m_level = level;
}

wtlog::LogLevel wtlog::Logger::level() const {
    return m_level;
}

void wtlog::Logger::log(std::string_view raw_msg) {
    std::string msg = enrich(raw_msg);
    if(msg.back() != '\n') {
        msg.push_back('\n');
    }
    std::string_view content(msg);
    for(ui64_t len = 0; len < content.size();) {
        len += m_carrier->store(content.substr(len));
        if(m_carrier->state() == details::Carrier::Status::ready) {
            send2Backend();
        }
    }
}

void wtlog::Logger::send2Backend() {
    auto carrier = m_carrier->transfer();
    m_distributor->distribute(carrier);
}

std::string wtlog::Logger::enrich(std::string_view raw_msg) {
    std::string msg(m_clock.timestamp());
    msg += m_logflags[m_level];
    msg += raw_msg;
    msg += '\n';
    return msg;
}

bool wtlog::logRemove(Pointer<Logger> logger) {
    return LogGenerator::instance().remove(logger);
}