/******************************************
 * @file:			LogBuffer.hpp		
 * @brief:			日志的缓存，以LogBuffer为基本单位进行操作
 * @version:		v0.0.0
 * @author:			cwt
 * @date:			2024-08-28
 ******************************************/
#ifndef LOGBUFFER_H__
#define LOGBUFFER_H__

#include <string_view>
#include <cstring>
#include <atomic>
#include "LogDef.hpp"

namespace wtlog {
namespace buffer {

enum class BuffStat : char {
	idle,
	writable,
	writing,
	readable,
	reading,
};

class Buffer {
public:
	explicit Buffer(const std::size_t capacity)
		: m_buff(new char[capacity] { }),
		m_capacity(capacity - 1) { }

	Buffer(const Buffer& logbuff)
		: m_buff(new char[logbuff.m_capacity + 1] { }),
		m_cur(logbuff.m_cur), 
		m_tail(logbuff.m_tail),
		m_capacity(logbuff.m_capacity) {
		m_stat.store(logbuff.m_stat);
		std::memcpy(m_buff, logbuff.m_buff, logbuff.m_capacity);
	}

	Buffer(Buffer&& logbuff) noexcept
		: m_buff(std::move(logbuff.m_buff)),
		m_cur(std::move(logbuff.m_cur)),
		m_tail(std::move(logbuff.m_tail)),
		m_capacity(std::move(logbuff.m_capacity)) {
		logbuff.m_buff = nullptr;
		this->m_stat.store(logbuff.m_stat);
	}

	Buffer& operator=(const Buffer& logbuff) {
		m_cur = logbuff.m_cur;
		m_tail = logbuff.m_tail;
		m_capacity = logbuff.m_capacity;
		delete[] m_buff;
		m_buff = new char[m_capacity + 1] { };
		std::memcpy(m_buff, logbuff.m_buff, logbuff.m_capacity);
	}

	Buffer& operator=(Buffer&& logbuff) noexcept {
		this->m_buff = std::move(logbuff.m_buff);
		logbuff.m_buff = nullptr;
		this->m_cur = std::move(logbuff.m_cur);
		this->m_tail = std::move(logbuff.m_tail);
		this->m_capacity = std::move(logbuff.m_capacity);
		this->m_stat.store(logbuff.m_stat);
		return *this;
	}

	~Buffer() {
		delete m_buff;
	}

public:
	/**
	 * @brief 将日志缓存进buffer中，并返回成功存放的日志的大小，
	 *		如果完整存放当前日志后的大小超过了一块buff的大小，会将当前这条日志截断，返回成功存放的字节数量，后续需要根据返回值自行截断再存储。
	 * @param log 当前要缓存的日志
	 * @return 成功缓存的字节数
	 */
	std::size_t store(std::string_view log) {
		std::size_t len = log.size() + m_tail > m_capacity ? m_capacity - m_tail : log.size();
		std::memcpy(m_buff + m_tail, log.data(), len);
		m_tail += len;
		return len;
	}

	/**
	 * @brief 从缓存buff中读取出日志，入参指定了读取日志的字节数量，超过了当前buff剩余的日志时，会将buff中的剩余日志全部读取出来。
	 * @param len 指定读取的字节长度
	 * @return 日志
	 */
	std::string_view load(const std::size_t len) {
		std::size_t length = len + m_cur > m_tail ? m_tail - m_cur : len;
		auto header = m_buff + m_cur;
		m_cur += length;
		return std::string_view(header, length);
	}

	/**
	 * @brief 清空buffer
	 */
	void clear() {
		m_cur = 0;
		m_tail = 0;
		m_stat = BuffStat::idle;
	}

	std::size_t length() const {
		return m_tail - m_cur;
	}

	bool empty() const {
		return m_cur == m_tail;
	}

	BuffStat state() const {
		return m_stat.load();
	}

	void setStat(BuffStat stat) {
		m_stat.store(stat);
	}

private:
	char* m_buff{ nullptr };			// 缓存的原始字符数组
	std::size_t m_cur{};				// 当前缓存的位置
	std::size_t m_tail{};				// 当前缓存使用的数量
	std::size_t m_capacity{};			// 缓存的容量

	std::atomic<BuffStat> m_stat{ BuffStat::idle };	// 当前buff的状态
};

}	// !namespace buffer
} 	// !namespace wtlog

#endif // !LOGBUFFER_H__

