/*********************************************************************************
 * @file		file_sinker.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-12-02 10:17
 **********************************************************************************/
#ifndef FILE_SINKER_H__
#define FILE_SINKER_H__

#include <wtlog/sinks/sinker.h>
#include <fstream>
#include <memory>
#include <filesystem>

namespace wtlog {
namespace sinks {

namespace fs = std::filesystem;

class SimpleFileSinker : public Sinker {
public:
    SimpleFileSinker(const std::string& filename, const fs::path& path);
    explicit SimpleFileSinker(const std::string& filename);

    ~SimpleFileSinker();

public:
    /**
     * @brief 将日志直接刷新到磁盘
     * @param message 日志信息
     */
    void flush(std::string_view message) override final;

private:
    /**
     * @brief 返回日志文件名的扩展名`.log`
     * @return 
     */
    constexpr std::string extension();

private:
    const fs::path m_filepath;
    std::ofstream m_out{};
};

class RotateFileSinker : public Sinker {
public:
    RotateFileSinker(const std::string& filename, ui64_t kb, const fs::path& path);
    RotateFileSinker(const std::string& filename, ui64_t kb);

    ~RotateFileSinker();

public:
    /**
     * @brief 将日志刷到磁盘
     * @param message 日志信息
     */
    void flush(std::string_view message) override final;

private:
    /**
     * @brief 为日志文件添加扩展名
     * @return 
     */
    constexpr std::string extension();

    /**
     * @brief 旋转日志文件
     */
    void rotateFile();

private:
    const ui64_t m_filesize;
    const fs::path m_filepath;
    std::ofstream m_out{};
    ui32_t m_seq{};
};

class DailyFileSinker : public Sinker {
public:
    DailyFileSinker(const std::string& filename, int hour_point, const fs::path& path);
    DailyFileSinker(const std::string& filename, int hour_point);

    ~DailyFileSinker();

public:
    /**
     * @brief 刷新日志文件到磁盘
     * @param message 日志信息
     */
    void flush(std::string_view message) override final;

private:
    /**
     * @brief 交换日志文件
     */
    void swapfile();

    /**
     * @brief 日志文件名添加扩展，包括日期和扩展名`.log`
     * @return 
     */
    std::string extension();

private:
    const i64_t oneday = 24 * 60 * 60;
    fs::path m_filepath;
    std::string m_prefix;
    std::time_t m_updatetime;
    std::ofstream m_out{};
};

} // !namespace sinker
} // !namespace wtlog

#endif // !FILE_SINKER_H__