/*********************************************************************************
 * @file		sinker.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-19 22:07
 **********************************************************************************/
#ifndef SINKER_H__
#define SINKER_H__

#include <wtlog/details/log_carrier.h>
#include <string_view>

namespace wtlog {
namespace sinks {

class SinkIdentifier {
protected:
    SinkIdentifier();

    SinkIdentifier(const SinkIdentifier&) = delete;

    SinkIdentifier(SinkIdentifier&& other);

public:
    ~SinkIdentifier() = default;

public:
    ui64_t identifier() const;

    SinkIdentifier& operator=(const SinkIdentifier&) = delete;

    SinkIdentifier& operator=(SinkIdentifier&&) = delete;

    bool operator==(const SinkIdentifier& other);

private:
    const ui64_t m_id;
};

class Sinker : public SinkIdentifier {
public:
    virtual ~Sinker() = default;
    
    /**
     * @brief 收集日志信息
     * @param carrier 日志信息的传输对象
     */
    virtual void collect(Pointer<details::Carrier> carrier) {
        flush(carrier->content());
    };

    /**
     * @brief 将日志输出到磁盘
     * @param message 日志信息
     */
    virtual void flush(std::string_view message) = 0;
};

} // !namespace sinks
} // !namespace wtlog


#endif // !SINKER_H__