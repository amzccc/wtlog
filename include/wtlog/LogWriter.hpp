/******************************************
 *@file			LogWriter.hpp
 *@brief		写入磁盘
 *@author		cwt
 *@version		v0.0.0
 *@date			2024-08-24 11:51
 ******************************************/
#ifndef LOGWRITER_HPP__
#define LOGWRITER_HPP__

#include <filesystem>

class LogWriter {
public:
    LogWriter(std::string_view path);

public:
    void write(std::string_view log);

private:
    void updateTime();

private:
    std::filesystem::path m_path{};
    std::string m_name{};
};

#endif //!LogWriter.hpp