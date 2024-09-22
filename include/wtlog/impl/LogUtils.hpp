/*********************************************************************************
 * @file		LogUtils.hpp
 * @brief		
 * @author		cwt
 * @version		v1.0.0
 * @date		2024-09-22 09:41
 **********************************************************************************/
#ifndef LOGUTILS_H__
#define LOGUTILS_H__

#include <chrono>
#include <string>
#include <cstring>

namespace wtlog {
namespace utils {

class Clock {
public:
	Clock()
		: m_timestamp(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) {
		std::memcpy(&m_tm, std::localtime(&m_timestamp), sizeof(std::tm));
		setDate(m_tm.tm_year, m_tm.tm_mon, m_tm.tm_mday);
		setHour(m_tm.tm_hour);
		setMinute(m_tm.tm_min);
		setSecond(m_tm.tm_sec);
	}

	~Clock() = default;

public:
	std::string timestamp() {
		using namespace std::chrono;
		auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        setMilliSec(now.count() % 1000);
		auto utc = now.count() / 1000;
		if(utc != m_timestamp) {
			auto local = std::localtime(&utc);
			if(local->tm_sec != m_tm.tm_sec) {
				setSecond(local->tm_sec);
			}
			if(local->tm_min != m_tm.tm_min) {
				setMinute(local->tm_min);
			}
			if(local->tm_hour != m_tm.tm_hour) {
				setHour(local->tm_hour);
			}
			if(local->tm_mday != m_tm.tm_mday
				|| local->tm_mon != m_tm.tm_mon
				|| local->tm_year != m_tm.tm_year) {
				setDate(local->tm_year, local->tm_mon, local->tm_mday);
			}
			m_timestamp = utc;
			std::memcpy(&m_tm, local, sizeof(std::tm));
		}
		return m_timestr;
	}

private:
	void setDate(int year, int month, int day) {
		year += 1900;
		month += 1;
		std::string str = std::to_string(year);
		m_timestr[0] = str[0];
		m_timestr[1] = str[1];
		m_timestr[2] = str[2];
		m_timestr[3] = str[3];
		str = month < 10 ? "0" + std::to_string(month) : std::to_string(month);
		m_timestr[5] = str[0];
		m_timestr[6] = str[1];
		str = day < 10 ? "0" + std::to_string(day) : std::to_string(day);
		m_timestr[8] = str[0];
		m_timestr[9] = str[1];
	}
	
	void setHour(int h) {
		std::string hour = h < 10 ? "0" + std::to_string(h) : std::to_string(h);
		m_timestr[11] = hour[0];
		m_timestr[12] = hour[1];
	}

	void setMinute(int min) {
		std::string minute = min < 10 ? "0" + std::to_string(min) : std::to_string(min);
		m_timestr[14] = minute[0];
		m_timestr[15] = minute[1];
	}

	void setSecond(int sec) {
		std::string second = sec < 10 ? "0" + std::to_string(sec) : std::to_string(sec);
		m_timestr[17] = second[0];
		m_timestr[18] = second[1];
	}

	void setMilliSec(int millisec) {
		std::string milli = millisec > 99 ? std::to_string(millisec)
			: millisec > 9 ? "0" + std::to_string(millisec)
			: "00" + std::to_string(millisec);
		m_timestr[20] = milli[0];
		m_timestr[21] = milli[1];
		m_timestr[22] = milli[2];
	}

private:
	time_t m_timestamp{};
	std::tm m_tm{};
	char m_timestr[24]{ "1900-01-01 00:00:00.000" };
};

}   // !namespace utils

}   // !namespace wtlog

#endif // !LogUtils.hpp