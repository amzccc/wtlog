/*********************************************************************************
 * @file		LogConfig.hpp
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-09-22 09:48
 **********************************************************************************/
#ifndef LOGCONFIG_H__
#define LOGCONFIG_H__

#include <chrono>
#include <string_view>
#include <filesystem>
#include <bitset>
#include "LogDef.hpp"

namespace wtlog {
namespace config {
class Configer {
protected:
    Configer(const Configer&) = delete;

    Configer& operator=(const Configer&) = delete;

    Configer(Configer&&) = delete;

    Configer& operator=(Configer&&) = delete;

public:
    Configer() = default;

    virtual ~Configer() = default;

public:
    /**
     * @brief 设置日志文件的路径，日志将会存放在@path文件夹下
     * @param path 指定日志存放路径
     * @return 返回设置结果，成功时返回Status::success。
     */
    Status setLogDir(const std::filesystem::path& path) {
        if(!std::filesystem::exists(path)) {
            if(!std::filesystem::create_directory(path)) {
                return Status::failure;
            }
        }
        m_dir = path;
        return Status::success;
    }

    std::filesystem::path logDir() const {
        return m_dir;
    }

    /**
     * @brief 设置日志对应等级开启还是关闭
     * @param level 日志等级
     * @param opt 是否开启，默认开启
     */
    void setLevel(Level level, bool opt = true) {
        switch(level) {
        case Level::none:
            if(opt) {
                m_levels.reset();
            }
            break;
        case Level::fatal:
            m_levels.set(1, opt);
            break;
        case Level::error:
            m_levels.set(2, opt);
            break;
        case Level::warning:
            m_levels.set(3, opt);
            break;
        case Level::info:
            m_levels.set(4, opt);
            break;
        case Level::trace:
            m_levels.set(5, opt);
            break;
        case Level::debug:
            m_levels.set(6, opt);
            break;
        default:
            break;
        }
    }

    bool isValid(Level level) const {
        switch(level) {
        case Level::fatal:
            return m_levels.test(1);
        case Level::error:
            return m_levels.test(2);
        case Level::warning:
            return m_levels.test(3);
        case Level::info:
            return m_levels.test(4);
        case Level::trace:
            return m_levels.test(5);
        case Level::debug:
            return m_levels.test(6);
        default:
            break;
        }
        return false;
    }

    /**
     * @brief 设置单个日志文件最大的大小
     * @param size 指定大小
     * @param unit 指定单位
     */
    void setFileSize(std::size_t size, FileUnit unit = FileUnit::KB) {
        if(unit == FileUnit::KB) {
            m_filesize = size * 1024;
        }
        else if(unit == FileUnit::MB) {
            m_filesize = size * 1024 * 1024;
        }
    }

    std::size_t filesize() const {
        return m_filesize;
    }

    /**
     * @brief 设置单个缓存块的最大容量
     * @param bytes 指定缓存块的容量，单位是字节
     */
    void setBuffCapacity(std::size_t bytes) {
        m_buffcapacity = bytes;
    }

    std::size_t buffcapacity() const {
        return m_buffcapacity;
    }

    /**
     * @brief 设置缓存块的最大数量，0表示关闭缓存，日志直接输入到日志文件中
     * @param quantity
     */
    void setBuffQuantity(std::size_t quantity) {
        m_buffcount = quantity;
    }

    std::size_t buffquantity() const {
        return m_buffcount;
    }

    /**
     * @brief 设置日志文件的旋转周期，最小时间单位为分钟，小于1分钟的间隔时间无效。
     *      特殊地，如果时间间隔设置为0表示日志文件不会旋转。
     * @param period 周期时间
     * @param unit 单位，默认为小时单位
     */
    void setRotationalPeriod(int period, TimeUnit unit = TimeUnit::hour) {
        using namespace std::chrono;
        switch(unit) {
        case TimeUnit::year:
            m_period = duration_cast<minutes>(years(period));
            break;
        case TimeUnit::month:
            m_period = duration_cast<minutes>(months(period));
            break;
        case TimeUnit::day:
            m_period = duration_cast<minutes>(days(period));
            break;
        case TimeUnit::hour:
            m_period = duration_cast<minutes>(hours(period));
            break;
        case TimeUnit::minute:
            m_period = minutes(period);
            break;
        case TimeUnit::second:
            m_period = seconds(period).count() > 60 ? duration_cast<minutes>(seconds(period)) : m_period;
            break;
        case TimeUnit::millsecond:
            m_period = milliseconds(period).count() > 60'000 ? duration_cast<minutes>(milliseconds(period)) : m_period;
            break;
        default:
            break;
        }
    }

    std::chrono::minutes period() const {
        return m_period;
    }

private:
    std::filesystem::path m_dir{ std::filesystem::current_path() /= "log" };  // 日志输出路径
    std::bitset<8> m_levels{ 2 };                     // 日志输出等级 0:none; 1:fatal; 2:error; 3:warning; 4:info; 5:trace; 6:debug
    std::size_t m_filesize{ 4 * 1024 * 1024 };      // 日志文件的大小
    std::chrono::minutes m_period{ std::chrono::minutes(24 * 60) };        // 日志文件的旋转间隔
    std::size_t m_buffcapacity{ 4096 };        // 单个缓存buffer的容量
    std::size_t m_buffcount{ 1 };       // 最多可以使用的buffer数量
};

}   // !namespace config
}   // !namespace wtlog

#endif // !LogConfig.hpp