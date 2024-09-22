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
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <utility>
#include "LogBuffer.hpp"
#include "LogScheduler.hpp"
#include "LogConfig.hpp"

namespace wtlog {
namespace writer {
namespace fs = std::filesystem;

class LocalWriterBase {
public:
	virtual ~LocalWriterBase() = default;
	
	virtual void doWrite(const std::shared_ptr<buffer::Buffer>& buff) = 0;
};

class AsyncLocalWriter : public LocalWriterBase {
public:
	AsyncLocalWriter(const std::shared_ptr<config::Configer>& config, std::string_view filename)
		: m_name(filename), m_config(config) {
		m_name += suffixName();
		if(!openFile(config->logDir())) {
			throw std::runtime_error("failed to open logfile.");
		}
		auto full_name = m_config->logDir();
		full_name /= m_name + extension();
		m_remain = m_config->filesize() - fs::file_size(full_name);
		m_worker = std::thread(&AsyncLocalWriter::writeLoop, this);
	}

	virtual ~AsyncLocalWriter() {
		m_working = false;
		{
			std::lock_guard<std::mutex> lg(m_mtx);
			m_signal.notify_one();
		}
		if(m_outfile.is_open()) {
			m_outfile.close();
		}
	}

public:
	void doWrite(const std::shared_ptr<buffer::Buffer>& buff) override {
		m_buffers.push_back(buff);
		std::lock_guard<std::mutex> lg(m_mtx);
		m_signal.notify_one();
	}

private:
	void writeLoop() {
		while(m_working) {
			std::unique_lock lock(m_mtx);
			m_signal.wait(lock, [this]() {
				return !m_buffers.empty() || !m_working;
			});
			writeTask();
		}
	}
	
	buffer::BuffStat writeTask() {
		std::size_t index = 0;
		std::size_t size = m_buffers.size();
		while(index < size) {
			auto len = std::min(m_remain, m_buffers.front()->length());
			m_outfile << m_buffers.front()->load(len);
			m_remain -= len;
			if(m_remain == 0) {
				rotate();
			}
			if(m_buffers.front()->length() == 0) {
				++index;
			}
		}
	}
	
	/**
	 * @brief 旋转日志文件
	 */
	bool rotate() {
		if(m_outfile.is_open()) {
			m_outfile.close();
		}
		auto oldname = m_config->logDir();
		oldname /= m_name;
		auto newname = oldname;
		newname += "_" + std::to_string(++m_seq);
		oldname += extension();
		newname += extension();
		fs::rename(oldname, newname);
		if(!openFile(m_config->logDir())) {
			return false;
		}
		auto full_name = m_config->logDir();
		full_name /= m_name + extension();
		m_remain = m_config->filesize() - fs::file_size(full_name);
		return true;
	}

	/**
	 * @brief 使用文件流打开日志文件
	 */
	bool openFile(const std::filesystem::path& dir) {
		if(!fs::exists(dir)) {
			if(!fs::create_directories(dir)) {
				return false;
			}
		}
		auto full_path = dir;
		full_path /= m_name + extension();
		m_outfile.open(full_path, std::ios::out | std::ios::app);
		return m_outfile.is_open();
	}

	/**
	 * @brief 文件名后缀
	 * @return 
	 */
	std::string suffixName() {
		using namespace std::chrono;
		auto time = system_clock::to_time_t(system_clock::now());
		auto s_tm = std::localtime(&time);
		return std::string("-").append(
			std::to_string(s_tm->tm_year)).append(
			std::to_string(s_tm->tm_mon)).append(
			std::to_string(s_tm->tm_mday)).append(
			std::to_string(s_tm->tm_hour)).append(
			std::to_string(s_tm->tm_min)).append(
			std::to_string(s_tm->tm_sec));
	}

	constexpr std::string extension() const {
		return ".log";
	}

private:
	std::shared_ptr<config::Configer> m_config{ nullptr };
	std::ofstream m_outfile;			// 输出文件流
	std::string m_name{};				// 日志前缀文件名
	std::size_t m_remain{};				// 当前日志文件剩余可写入字节数量
	int m_seq{ 0 };						// 日志文件序号

	std::vector<std::shared_ptr<buffer::Buffer>> m_buffers{};
	std::thread m_worker{};
	std::mutex m_mtx{};
	std::condition_variable m_signal{};
	std::atomic<bool> m_working{ true };
};

}	// !namespace writer
} 	// !namespace wtlog

#endif //!LogWriter.hpp