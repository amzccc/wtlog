#include <wtlog/sinks/file_sinker.h>
#include <wtlog/details/log_utils.h>
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;

wtlog::sinks::SimpleFileSinker::SimpleFileSinker(const std::string& filename, const fs::path& path) {
    if(!fs::exists(path)) {
        fs::create_directories(path);
    }
    auto absolute = path;
    absolute /= filename;
    absolute += extension();
    m_out.open(absolute, std::ios::app);
}

wtlog::sinks::SimpleFileSinker::SimpleFileSinker(const std::string& filename)
    : SimpleFileSinker(filename, fs::current_path() / "log") { }

wtlog::sinks::SimpleFileSinker::~SimpleFileSinker() {
    if(m_out.is_open()) {
        m_out.close();
    }
}

void wtlog::sinks::SimpleFileSinker::flush() {
    if(!m_out.is_open()) {
        return;
    }
    m_out.write(m_carrier->content().data(), m_carrier->content().size());
}

std::string wtlog::sinks::SimpleFileSinker::extension() {
    return "-" + std::to_string(wtlog::utils::plaintime(std::time(nullptr))) + ".log";
}

wtlog::sinks::RotateFileSinker::RotateFileSinker(const std::string& filename, ui64_t filesize, const fs::path& path)
    : SimpleFileSinker(filename, path),
    m_filesize(filesize) { }

wtlog::sinks::RotateFileSinker::RotateFileSinker(const std::string& filename, ui64_t filesize)
    : RotateFileSinker(filename, filesize, fs::current_path() / "log") { }

wtlog::sinks::DailyFileSinker::DailyFileSinker(const std::string& filename, int hour_point, const fs::path& path)
    : SimpleFileSinker(filename, path),
    m_dir(path),
    m_filename(filename) {
    std::time_t time = std::time(nullptr);
    std::tm local;
    utils::localtime(time, local);
    local.tm_sec = 0;
    local.tm_min = 0;
    auto cur_hour = local.tm_hour;
    local.tm_hour = hour_point;
    m_updatetime = std::mktime(&local);
    if(cur_hour >= hour_point) {
        m_updatetime += oneday;
    }
}

wtlog::sinks::DailyFileSinker::DailyFileSinker(const std::string& filename, int hour_point)
    : DailyFileSinker(filename, hour_point, fs::current_path() / "log") { }

void wtlog::sinks::DailyFileSinker::flush() {
    m_out.write(m_carrier->content().data(), m_carrier->content().size());
    auto current = std::time(nullptr);
    if(current >= m_updatetime) {
        swapFile();
        while(current >= m_updatetime) {
            m_updatetime += oneday;
        }
    }
}

void wtlog::sinks::DailyFileSinker::swapFile() {
    m_out.close();
    if(!fs::exists(m_dir)) {
        fs::create_directories(m_dir);
    }
    fs::path full_name = m_dir / m_filename;
    full_name += extension();
    m_out.open(full_name, std::ios::out);
}
