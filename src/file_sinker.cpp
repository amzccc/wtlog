#include <wtlog/sinks/file_sinker.h>
#include <wtlog/details/log_utils.h>
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;

wtlog::sinks::SimpleFileSinker::SimpleFileSinker(const std::string& filename, const fs::path& directory)
    : m_filepath((directory / filename).replace_extension(extension())) {
    if(!fs::exists(directory)) {
        fs::create_directories(directory);
    }
}

wtlog::sinks::SimpleFileSinker::SimpleFileSinker(const std::string& filename)
    : SimpleFileSinker(filename, fs::current_path() / "log") { }

wtlog::sinks::SimpleFileSinker::~SimpleFileSinker() {
    if(m_out.is_open()) {
        m_out.close();
    }
}

void wtlog::sinks::SimpleFileSinker::flush(std::string_view message) {
    if(message.empty()) {
        return;
    }
    if(!fs::exists(m_filepath.parent_path())) {
        fs::create_directories(m_filepath.parent_path());
    }
    m_out.open(m_filepath, std::ios::app);
    m_out.write(message.data(), message.size());
    m_out.close();
}

constexpr std::string wtlog::sinks::SimpleFileSinker::extension() {
    return ".log";
}

/*===================================================================================================================*/

wtlog::sinks::RotateFileSinker::RotateFileSinker(const std::string& filename, ui64_t kb, const fs::path& directory)
    : m_filesize(kb * 1024),
    m_filepath((directory / filename).append(std::to_string(wtlog::utils::plaintime(std::time(nullptr)))).replace_extension(extension())) {
    if(!fs::exists(directory)) {
        fs::create_directories(directory);
    }
}

wtlog::sinks::RotateFileSinker::RotateFileSinker(const std::string& filename, ui64_t kb)
    : RotateFileSinker(filename, kb, fs::current_path() / "log") { }

wtlog::sinks::RotateFileSinker::~RotateFileSinker() {
    if(m_out.is_open()) {
        m_out.close();
    }
}

void wtlog::sinks::RotateFileSinker::flush(std::string_view message) {
    if(message.empty()) {
        return;
    }
    auto m_cursize = fs::file_size(m_filepath);
    if(!fs::exists(m_filepath.parent_path())) {
        fs::create_directories(m_filepath.parent_path());
    }
    m_out.open(m_filepath, std::ios::app);
    ui64_t len = message.size();
    if(len + m_cursize > m_filesize) {
        len = m_filesize - m_cursize;
        len = message.substr(0, len).find_last_of('\n') + 1;
    }
    m_out.write(message.data(), len);
    m_out.close();
    if(len < message.size() || m_cursize + len == m_filesize) {
        rotateFile();
        return flush(message.substr(len, message.size() - len));
    }
}

constexpr std::string wtlog::sinks::RotateFileSinker::extension() {
    return ".log";
}

void wtlog::sinks::RotateFileSinker::rotateFile() {
    if(fs::exists(m_filepath)) {
        auto newname = m_filepath;
        newname.replace_filename(m_filepath.stem()).append("_" + std::to_string(++m_seq)).replace_extension(extension());
        fs::rename(m_filepath, newname);
    }
}

/*=================================================================================================================*/

wtlog::sinks::DailyFileSinker::DailyFileSinker(const std::string& filename, int hour_point, const fs::path& directory)
    : m_filepath((directory / filename).append(extension())),
    m_prefix(filename) {
    if(!fs::exists(directory)) {
        fs::create_directories(directory);
    }
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

wtlog::sinks::DailyFileSinker::~DailyFileSinker() {
    if(m_out.is_open()) {
        m_out.close();
    }
}

void wtlog::sinks::DailyFileSinker::flush(std::string_view message) {
    if(!fs::exists(m_filepath.parent_path())) {
        fs::create_directories(m_filepath.parent_path());
    }
    m_out.open(m_filepath, std::ios::app);
    m_out.write(message.data(), message.size());
    m_out.close();
    auto current = std::time(nullptr);
    if(current >= m_updatetime) {
        swapfile();
        while(current >= m_updatetime) {
            m_updatetime += oneday;
        }
    }
}

void wtlog::sinks::DailyFileSinker::swapfile() {
    m_filepath.replace_filename(m_prefix + extension());
}

std::string wtlog::sinks::DailyFileSinker::extension() {
    return std::to_string(wtlog::utils::plaintime(std::time(nullptr))) + ".log";
}
