/*********************************************************************************
 * @file		sink_distributor.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-20 15:34
 **********************************************************************************/
#ifndef SINK_DISTRIBUTOR_H__
#define SINK_DISTRIBUTOR_H__

#include <wtlog/details/log_typedef.h>
#include <wtlog/sinks/sinker.h>
#include <unordered_map>
#include <vector>

namespace wtlog {
namespace sinks {

class SinkDistributor {
public:
    SinkDistributor() = default;

    SinkDistributor(const SinkDistributor&) = delete;

    SinkDistributor(SinkDistributor&&) = delete;

public:
    ~SinkDistributor() = default;

public:
    /**
     * @brief 向分流器中注册一个日志接收端。
     * @param sinker 要注册的日志收集槽
     * @return 返回分配给这个接收端的唯一标识
     */
    void registerSink(Pointer<Sinker> sinker);

    /**
     * @brief 取消一个日志接收端的注册状态，当接收端被取消了注册就不会再收到日志消息。
     * @param sinker 要取消注册的日志接收端
     * @return 返回被成功取消注册的接收端的标识，当返回0时表示没有接收端被取消。
     */
    void unregisterSink(Pointer<Sinker> sinker);

    /**
     * @brief 向所有接收端标识对应的日志接收端分发日志信息
     * @param carrier 要分发的日志信息
     */
    virtual void distribute(Pointer<details::Carrier> carrier);

    /**
     * @brief 设置接收器的掩码，置为false时将不会接收日志
     * @param sinker 要设置的日志接收器
     * @param mask 掩码
     */
    void mask(Pointer<Sinker> sinker, bool mask);

protected:
    virtual void doDistribute(const Pointer<details::Carrier>& carrier);

protected:
    std::vector<Pointer<Sinker>> m_sinks{};
    std::unordered_map<ui64_t, bool> m_mask{};
};

} // namespace sinks
} // namespace wtlog

#endif // !SINK_DISTRIBUTOR_H__