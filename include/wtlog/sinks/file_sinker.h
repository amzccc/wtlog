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
     */
    void flush() override;

protected:
    virtual std::string extension();

protected:
    std::ofstream m_out{};
};

class RotateFileSinker : public SimpleFileSinker {
public:
    RotateFileSinker(const std::string& filename, ui64_t filesize, const fs::path& path);
    RotateFileSinker(const std::string& filename, ui64_t filesize);

public:
    // void flush() override;

private:
    // std::string extension() override;

private:
    ui64_t m_filesize;
};

class DailyFileSinker : public SimpleFileSinker {
public:
    DailyFileSinker(const std::string& filename, int hour_point, const fs::path& path);
    DailyFileSinker(const std::string& filename, int hour_point);

public:
    void flush() override;

private:
    void swapFile();

private:
    const fs::path m_dir;
    const std::string m_filename;
    const i64_t oneday = 24 * 60 * 60;
    std::time_t m_updatetime;
};

} // !namespace sinker
} // !namespace wtlog

#endif // !FILE_SINKER_H__