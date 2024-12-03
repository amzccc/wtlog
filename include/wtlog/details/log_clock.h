/*********************************************************************************
 * @file		log_clock.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-22 15:55
 **********************************************************************************/
#ifndef LOG_CLOCK_H__
#define LOG_CLOCK_H__

#include "log_typedef.h"
#include <chrono>
#include <vector>
#include <string>

namespace wtlog {
namespace utils {

class Clock {
public:
    enum class Unit : char {
        day = 1,
        hour = 2,
        minute = 3,
        second = 4,
        millisecond = 5,
    };
    
public:
    Clock(Unit precision = Unit::millisecond);

    ~Clock() = default;

public:
    /**
     * @brief 返回当前时间的时间戳
     * @return 
     */
    std::string timestamp();

    /**
     * @brief 设置返回的时间戳时间精度
     * @param precision 时间精度的最小单位
     */
    void setPrecision(Unit precision);

private:
    /**
     * @brief 返回本地时间的tm结构
     * @param timer time_t类型值
     * @param local tm结构的本地时间
     */
    void localTime(time_t timer, tm& local);

    /**
     * @brief 设置日期
     * @param year 年
     * @param month 月
     * @param day 日
     */
    void setDate(i32_t year, i32_t month, i32_t day);

    /**
     * @brief 设置小时数
     * @param hour 小时
     */
    void setHour(i32_t hour);

    /**
     * @brief 设置分钟数
     * @param minute 分钟
     */
    void setMinute(i32_t minute);

    /**
     * @brief 设置秒数
     * @param second 秒
     */
    void setSecond(i32_t second);

    /**
     * @brief 设置毫秒数
     * @param millisecond 毫秒
     */
    void setMillisecond(i32_t millisecond);

    /**
     * @brief 显示时间戳字符串
     * @return 
     */
    std::string display();

    /**
     * @brief 初始化时钟的数字映射
     */
    void initializeClockNumber();

private:
    inline static std::vector<std::string> s_numbers{};
    i32_t m_second_offset{};
    i32_t m_minute_offset{};
    i32_t m_hour_offset{};
    i64_t m_startpoint{};
    std::string m_timestamp{ "0000-00-00 00:00:00.000" };
    Unit m_precision{ Unit::millisecond };
};

} // !namespace utils
} // !namespace wtlog


#endif // !LOG_CLOCK_H__