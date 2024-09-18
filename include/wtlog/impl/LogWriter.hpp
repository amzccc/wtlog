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
#include "LogBuffer.hpp"

namespace wtlog {
namespace writer {

using namespace common;

class LogWriter {
public:
	explicit LogWriter(std::string_view prefix);

	virtual ~LogWriter();

public:
	/**
	 * @brief 将日志写入磁盘
	 * @param log 待写入的日志
	 */
	void write(std::string_view log);

	/**
	 * @brief 异步写入日志
	 * @param buffer 
	 * @param handler 
	 */
	void asyncWrite();

private:
	/**
	 * @brief 旋转日志文件
	 */
	void rotate();

	/**
	 * @brief 使用文件流打开日志文件
	 */
	void openFile();

	/**
	 * @brief 拼接日志文件的完整文件名
	 * @return 
	 */
	std::string fullName();

private:
	std::string m_prefix{};				// 日志文件名前缀
	std::filesystem::path m_fullpath{};	// 日志文件的完整路径

	std::ofstream m_out;				// 输出文件流
	std::size_t m_filebytes{ 0 };		// 当前打开的日志文件已写入的字节数量
};

}	// !namespace writer
} 	// !namespace wtlog

#endif //!LogWriter.hpp