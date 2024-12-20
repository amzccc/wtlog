#include <wtlog/details/log_utils.h>
#include <atomic>
#include <cstring>

using namespace std::chrono;
std::array<std::string, 1000> wtlog::utils::Clock::s_numbers{};
std::atomic<bool> wtlog::utils::Clock::is_initialized(false);

wtlog::utils::Clock::Clock(Unit precision)
    : m_precision(precision),
      m_startpoint(system_clock::to_time_t(system_clock::now())) {
    initializeClockNumber();
    std::tm local;
    localtime(m_startpoint, local);
    setDate(local.tm_year, local.tm_mon, local.tm_mday);
    setHour(local.tm_hour);
    setMinute(local.tm_min);
    setSecond(local.tm_sec);
    m_second_offset = local.tm_sec;
    m_minute_offset = local.tm_min;
    m_hour_offset = local.tm_hour;
}

void wtlog::utils::Clock::setDate(i32_t year, i32_t month, i32_t day) {
    int pos = 0;
    std::string str = std::to_string(year + 1900);
    for(int i = 0; i < 4; i++) {
        m_timestamp[pos++] = str[i];
    }
    ++pos;
    const auto& mon = s_numbers[month + 1];
    for(int i = 1; i < 3; i++) {
        m_timestamp[pos++] = mon[i];
    }
    ++pos;
    const auto& d = s_numbers[day];
    for(int i = 1; i < 3; i++) {
        m_timestamp[pos++] = d[i];
    }
}

void wtlog::utils::Clock::setHour(i32_t hour) {
    if(m_precision >= Unit::hour) {
        int pos = 11;
        const auto& number = s_numbers[hour];
        for(int i = 1; i < 3; i++) {
            m_timestamp[pos++] = number[i];
        }
    }
}

void wtlog::utils::Clock::setMinute(i32_t minute) {
    if(m_precision >= Unit::minute) {
        int pos = 14;
        const auto& number = s_numbers[minute];
        for(int i = 1; i < 3; i++) {
            m_timestamp[pos++] = number[i];
        }
    }
}

void wtlog::utils::Clock::setSecond(i32_t second) {
    if(m_precision >= Unit::second) {
        int pos = 17;
        const auto& number = s_numbers[second];
        for(int i = 1; i < 3; i++) {
            m_timestamp[pos++] = number[i];
        }
    }
}

void wtlog::utils::Clock::setMillisecond(i32_t millisecond) {
    if(m_precision == Unit::millisecond) {
        int pos = 20;
        const auto& number = s_numbers[millisecond];
        for(int i = 0; i < 3; i++) {
            m_timestamp[pos++] = number[i];
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
        localtime(timer, local);
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
    ui64_t cnt = 0;
    switch(m_precision) {
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
        cnt = 23;
        break;
    default:
        break;
    }
    return std::string(m_timestamp).substr(0, cnt);
}

void wtlog::utils::Clock::initializeClockNumber() {
    if(is_initialized) {
        return;
    }
    s_numbers.fill("000");
    for(int i = 0; i < s_numbers.size(); i++) {
        int offset = i < 10 ? 2 : i < 100 ? 1 : 0;
        std::strcpy(s_numbers[i].data() + offset, std::to_string(i).c_str());
    }
    is_initialized.store(true);
}

void wtlog::utils::localtime(time_t timer, tm& local) {
#ifdef __linux__
    localtime_r(&timer, &local);
#endif
#ifdef WIN32
    localtime_s(&local, &timer);
#endif
}

wtlog::ui64_t wtlog::utils::plaintime(std::time_t utc) {
    std::tm local;
    localtime(utc, local);
    return (local.tm_year + 1900) * 10000000000 + (local.tm_mon + 1) * 100000000 +
           local.tm_mday * 1000000 + local.tm_hour * 10000 + local.tm_min * 100 + local.tm_sec;
}