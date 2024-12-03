/*********************************************************************************
 * @file		sinker.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-19 22:07
 **********************************************************************************/
#ifndef SINKER_H__
#define SINKER_H__

#include "wtlog/details/log_carrier.h"
#include <string_view>

namespace wtlog {
namespace sinks {

class Sinker {
public:
    virtual ~Sinker() = default;
    
    /**
     * @brief 收集日志信息
     * @param carrier 日志信息的传输对象
     */
    virtual void collect(Pointer<details::Carrier> carrier) {
        m_carrier = carrier;
        flush();
    };

    /**
     * @brief 将日志输出到磁盘
     */
    virtual void flush() = 0;

protected:
    Pointer<details::Carrier> m_carrier{ nullptr };
};

} // !namespace sinks
} // !namespace wtlog


#endif // !SINKER_H__