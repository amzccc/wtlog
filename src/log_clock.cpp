#include "wtlog/details/log_clock.h"
#include <map>
#include <mutex>

using namespace std::chrono;


wtlog::utils::Clock::Clock(Unit precision)
    : m_precision(precision),
    m_startpoint(system_clock::to_time_t(system_clock::now())) {
    initializeClockNumber();
    std::tm local;
    localTime(m_startpoint, local);
    setDate(local.tm_year, local.tm_mon, local.tm_mday);
    setHour(local.tm_hour);
    setMinute(local.tm_min);
    setSecond(local.tm_sec);
    m_second_offset = local.tm_sec;
    m_minute_offset = local.tm_min;
    m_hour_offset = local.tm_hour;
}

void wtlog::utils::Clock::localTime(time_t timer, tm& local) {
#ifdef __linux__
    localtime_r(&timer, &local);
#endif
#ifdef WIN32
    localtime_s(&local, &timer);
#endif
}

void wtlog::utils::Clock::setDate(i32_t year, i32_t month, i32_t day) {
    i32_t pos = 0;
    std::string str = std::to_string(year + 1900);
    for(i32_t i = 0; i < 4; i++) {
        m_timestamp[pos++] = str[i];
    }
    ++pos;
    for(i32_t i = 1; i < 3; i++) {
        m_timestamp[pos++] = s_numbers[month + 1][i];
    }
    ++pos;
    for(i32_t i = 1; i < 3; i++) {
        m_timestamp[pos++] = s_numbers[day][i];
    }
}

void wtlog::utils::Clock::setHour(i32_t hour) {
    if(m_precision >= Unit::hour) {
        i32_t pos = 11;
        for(i32_t i = 1; i < 3; i++) {
            m_timestamp[pos++] = s_numbers[hour][i];
        }
    }
}

void wtlog::utils::Clock::setMinute(i32_t minute) {
    if(m_precision >= Unit::minute) {
        i32_t pos = 14;
        for(i32_t i = 1; i < 3; i++) {
            m_timestamp[pos++] = s_numbers[minute][i];
        }
    }
}

void wtlog::utils::Clock::setSecond(i32_t second) {
    if(m_precision >= Unit::second) {

        i32_t pos = 17;
        for(i32_t i = 1; i < 3; i++) {
            m_timestamp[pos++] = s_numbers[second][i];
        }
    }
}

void wtlog::utils::Clock::setMillisecond(i32_t millisecond) {
    if(m_precision == Unit::millisecond) {
        i32_t pos = 20;
        for(i32_t i = 0; i < 3; i++) {
            m_timestamp[pos++] = s_numbers[millisecond][i];
        }
    }
}

std::string wtlog::utils::Clock::timestamp() {
    auto cur = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    setMillisecond(cur.count() % 1000);
    i32_t interval = duration_cast<seconds>(cur).count() - m_startpoint;
    do {
        if(interval == 0) {
            break;
        }
        setSecond((m_second_offset + interval) % 60);
        interval /= 60;
        if(interval == 0) {
            break;
        }
        setMinute((m_minute_offset + interval) % 60);
        interval /= 60;
        if(interval == 0) {
            break;
        }
        setHour((m_hour_offset + interval) % 24);
        interval /= 24;
        if(interval == 0) {
            break;
        }
        tm local;
        time_t timer = cur.count() / 1000;
        localTime(timer, local);
        setDate(local.tm_year, local.tm_mon, local.tm_mday);
        m_hour_offset = local.tm_hour;
        m_minute_offset = local.tm_min;
        m_second_offset = local.tm_sec;
        m_startpoint = duration_cast<seconds>(cur).count();
    } while(false);
    return display();
}

void wtlog::utils::Clock::setPrecision(Clock::Unit precision) {
    m_precision = precision;
}

std::string wtlog::utils::Clock::display() {
    i64_t cnt = 0;
    switch(m_precision)
    {
    case Unit::day:
        cnt = 10;
        break;
    case Unit::hour:
        cnt = 13;
        break;
    case Unit::minute:
        cnt = 16;
        break;
    case Unit::second:
        cnt = 19;
        break;
    case Unit::millisecond:
        cnt = 24;
        break;
    default:
        break;
    }
    return m_timestamp.substr(0, cnt);
}

void wtlog::utils::Clock::initializeClockNumber() {
    static std::mutex s_mtx;
    std::lock_guard lg(s_mtx);
    if(s_numbers.empty()) {
        for(int i = 0; i < 1000; i++) {
            s_numbers.push_back(i < 10 ? "00" + std::to_string(i) :
                                i < 100 ? "0" + std::to_string(i) :
                                                std::to_string(i));
        }
    }
}