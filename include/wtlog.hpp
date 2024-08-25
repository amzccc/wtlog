/******************************************
 *@file		    wtlog.hpp
 *@brief		wtlog接口头文件
 *@author		cwt
 *@version		v0.0.0
 *@date		    2024-08-23 11:25
 ******************************************/
#ifndef LOGGERPLUSPLUS_HPP__
#define LOGGERPLUSPLUS_HPP__

#include "wtlog/LogDef.hpp"
#include "wtlog/LogImpl.hpp"
#include <string_view>
#include <memory>
#include <format>


namespace wtlog {

class Logger {
protected:
    Logger() = default;

    Logger(const Logger&) = delete;

    Logger& operator=(const Logger&) noexcept = delete;

public:
    Logger(Logger&&) noexcept;

    Logger& operator=(Logger&&) noexcept;

    ~Logger() = default;

public:
    static std::shared_ptr<Logger> instance();

    static Logger& instanceRef();

    template<typename... Args>
    void log(Level lv, std::string_view format, Args&&... args) {
        m_impl->log(lv, std::forward<std::string_view>(format), std::forward<Args>(args)...);
    }
    
private:
    std::unique_ptr<LogImpl<int>> m_impl{ nullptr };
};



}   //! namespace wtlog


#endif //!LOGGERPLUSPLUS_hpp