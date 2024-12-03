/*********************************************************************************
 * @file		log_typedef.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-13 18:23
 **********************************************************************************/
#ifndef LOG_TYPEDEF_H__
#define LOG_TYPEDEF_H__


#include <memory>

namespace wtlog {

using byte_t = unsigned char;
using i16_t = short;
using ui16_t = unsigned short;
using i32_t = int;
using ui32_t = unsigned int;
using i64_t = std::ptrdiff_t;
using ui64_t = std::size_t;

template<typename T>
using Pointer = std::shared_ptr<T>;

template<typename T>
using Unique = std::unique_ptr<T>;

enum class LogLevel : char {
    trace,
    debug,
    info,
    warn,
    error,
    fatal,
    hide
};

enum class ResultCode : int {
    succeed = 0,
    failure,
    overflow,
    full,
};



} // !namespace wtlog

#endif // !LOG_TYPEDEF_H__