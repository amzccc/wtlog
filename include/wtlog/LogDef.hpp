/******************************************
 *@file		    LogDef.hpp
 *@brief		wtlog会使用到的参数
 *@author		cwt
 *@version		v0.0.0
 *@date		    2024-08-23 11:23
 ******************************************/
#ifndef LOGDEF_HPP__
#define LOGDEF_HPP__

#include <cstdint>
#include <typeinfo>

namespace wtlog {

enum class Level : char {
    trace = 0,
    debug = 1,
    info = 2,
    warning = 3,
    error = 4,
    fatal = 5
};

class LogConfig {
protected:
    LogConfig() = default;

    LogConfig(const LogConfig&) = delete;

    LogConfig& operator=(const LogConfig&) = delete;

    LogConfig(LogConfig&&) = delete;

    LogConfig& operator=(LogConfig&&) = delete;

public:
    ~LogConfig() = default;

public:
    LogConfig& instance();

public:
    inline static Level log_level{ Level::error };
    inline static std::size_t log_size{ 4 * 1024 * 1024 };

};

}   //!namespace wtlog

#endif //!LogDef.hpp