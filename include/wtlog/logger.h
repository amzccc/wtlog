/*********************************************************************************
 * @file		logger.h
 * @brief
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-13 16:39
 **********************************************************************************/
#ifndef LOGGER_H__
#define LOGGER_H__

#include <wtlog/details/log_carrier.h>
#include <wtlog/details/log_utils.h>
#include <wtlog/sinks/sink_distributor.h>

#include <format>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace wtlog {
/**
 * @brief 同步logger
 */
class Logger {
public:
    Logger() = default;

    virtual ~Logger();

public:
    template <typename SinkerT, typename... Args,
              typename = std::enable_if_t<std::is_base_of_v<sinks::Sinker, SinkerT> &&
                                          std::is_constructible_v<SinkerT, Args...>>>
    Pointer<SinkerT> createSinker(Args&&... args) {
        auto pointer = std::make_shared<SinkerT>(std::forward<Args>(args)...);
        attachSinker(pointer);
        return pointer;
    }

    /**
     * @brief 绑定一个日志接收端
     * @param sinker 待绑定的日志接收端
     */
    void attachSinker(Pointer<sinks::Sinker> sinker);

    /**
     * @brief 解绑一个日志接收端
     * @param sinker 待解绑的接收端
     */
    void detachSinker(Pointer<sinks::Sinker> sinker);

    /**
     * @brief 设置日志包装类
     * @param carrier 指向日志包装类的独占指针
     */
    template <typename CarrierT, typename... Args,
              typename = std::enable_if<std::is_base_of_v<details::Carrier, CarrierT> &&
                                        std::is_constructible_v<CarrierT, Args...>>>
    void setCarrier(Args&&... args) {
        m_carrier = std::make_shared<CarrierT>(std::forward<Args>(args)...);
    }

    /**
     * @brief 设置日志时间的最小精度单位，将会影响每条日志显示的打印时间
     * @param precision 时间精度单位
     */
    void setTimePrecision(wtlog::utils::Clock::Unit precision);

    /**
     * @brief 设置日志等级
     * @param level 日志等级
     */
    void setLevel(const LogLevel level);

    /**
     * @brief 当前logger设置的日志
     * @return
     */
    LogLevel level() const;

    /**
     * @brief 日志信息入口
     * @param raw_msg 原始日志信息
     */
    virtual void log(std::string_view raw_msg);

protected:
    /**
     * @brief 将日志发送给后端处理
     */
    void send2Backend();

    /**
     * @brief 扩展日志，添加日志打印时间和日志等级标识
     * @param raw_msg 日志消息
     * @return 经过扩展加工后的日志
     */
    std::string enrich(std::string_view raw_msg);

protected:
    LogLevel m_level{LogLevel::error};
    wtlog::utils::Clock m_clock{};
    Pointer<details::Carrier> m_carrier{std::make_shared<details::SimpleCarrier>()};
    Pointer<sinks::SinkDistributor> m_distributor{std::make_shared<sinks::SinkDistributor>()};
    static std::map<LogLevel, std::string> m_logflags;
};

/*-------------------------log generator------------------------------*/
class LogGenerator {
private:
    LogGenerator() = default;

    LogGenerator(const LogGenerator&) = delete;

    LogGenerator(LogGenerator&&) noexcept = delete;

    ~LogGenerator() = default;

public:
    /**
     * @brief 返回日志工厂的实例
     * @return
     */
    static LogGenerator& instance();

    /**
     * @brief 构造一个日志对象
     * @tparam LogT 要构造的日志对象类型
     * @tparam ...Args 日志对象会使用到的构造入参参数类型
     * @tparam
     * @param ...args 日志对象的构造入参
     * @return 返回指向构造出的日志对象的指针
     */
    template <typename LogT, typename... Args,
              typename = std::enable_if_t<std::is_constructible_v<LogT, Args...>>>
    Pointer<LogT> create(Args&&... args) {
        return m_loggers.emplace_back(std::make_shared<LogT>(std::forward<Args>(args)...));
    }

    bool remove(const Pointer<Logger>& logger);

    /**
     * @brief 打印日志接口
     * @tparam ...Args 日志消息的参数类型
     * @param level 日志等级
     * @param fmt 日志消息的格式化字符串
     * @param ...args 日志消息的参数
     */
    template <typename... Args>
    void log(const LogLevel level, std::string_view fmt, Args&&... args) {
        auto message = std::vformat(fmt, std::make_format_args(std::forward<Args&>(args)...));
        for (auto& logger : m_loggers) {
            if (logger->level() <= level) {
                logger->log(message);
            }
        }
    }

private:
    inline static std::vector<Pointer<Logger>> m_loggers{};
};

template <typename LogT = wtlog::Logger, typename... Args>
Pointer<LogT> logCreate(Args&&... args) {
    return LogGenerator::instance().create<LogT>(std::forward<Args>(args)...);
}

bool logRemove(Pointer<Logger> logger);

/**
 * @brief 打印trace等级日志
 * @tparam Args 日志参数
 * @param fmt 要打印的日志信息
 * @param args 日志格式化信息
 */
template <typename... Args>
void trace(const std::string& fmt, Args&&... args) {
    LogGenerator::instance().log(LogLevel::trace, fmt, std::forward<Args>(args)...);
}

/**
 * @brief 打印debug等级日志
 * @tparam Args 日志参数
 * @param fmt 要打印的日志信息
 * @param args 日志格式化信息
 */
template <typename... Args>
void debug(const std::string& fmt, Args&&... args) {
    LogGenerator::instance().log(LogLevel::debug, fmt, std::forward<Args>(args)...);
}

/**
 * @brief 打印info等级日志
 * @tparam Args 日志参数
 * @param fmt 要打印的日志信息
 * @param args 日志格式化信息
 */
template <typename... Args>
void info(const std::string& fmt, Args&&... args) {
    LogGenerator::instance().log(LogLevel::info, fmt, std::forward<Args>(args)...);
}

/**
 * @brief 打印trace等级日志
 * @tparam Args 日志参数
 * @param fmt 要打印的日志信息
 * @param args 日志格式化信息
 */
template <typename... Args>
void warn(const std::string& fmt, Args&&... args) {
    LogGenerator::instance().log(LogLevel::warn, fmt, std::forward<Args>(args)...);
}

/**
 * @brief 打印error等级日志
 * @tparam Args 日志参数
 * @param fmt 要打印的日志信息
 * @param args 日志格式化信息
 */
template <typename... Args>
void error(const std::string& fmt, Args&&... args) {
    LogGenerator::instance().log(LogLevel::error, fmt, std::forward<Args>(args)...);
}

/**
 * @brief 打印fatal等级日志
 * @tparam Args 日志参数
 * @param fmt 要打印的日志信息
 * @param args 日志格式化信息
 */
template <typename... Args>
void fatal(const std::string& fmt, Args&&... args) {
    LogGenerator::instance().log(LogLevel::fatal, fmt, std::forward<Args>(args)...);
}

} // namespace wtlog

#endif // !LOGGER_H__