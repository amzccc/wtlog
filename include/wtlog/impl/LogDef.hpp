/******************************************
 *@file		    LogDef.hpp
 *@brief		wtlog会使用到的参数
 *@author		cwt
 *@version		v0.0.0
 *@date		    2024-08-23 11:23
 ******************************************/
#ifndef LOGDEF_HPP__
#define LOGDEF_HPP__

#include <limits>


namespace wtlog {
/**
 * @brief 日志等级
 */
enum class Level : char {
    none,
    fatal,
    error,
    warning,
    info,
    trace,
    debug,
};

/**
 * @brief 执行结果
 */
enum class Status : int {
    invalid = std::numeric_limits<int>::min(),
    success = 0,
    failure = 1,
    skip = 2,
};

enum class FileUnit : bool {
    KB,
    MB
};

enum class TimeUnit : char {
    year,
    month,
    day,
    hour,
    minute,
    second,
    millsecond,
};

}   // !namespace wtlog

#endif //!LogDef.hpp