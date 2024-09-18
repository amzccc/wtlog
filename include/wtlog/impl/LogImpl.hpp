/******************************************
 *@file		    LogImpl.hpp
 *@brief		Logger的实现类
 *@author		cwt
 *@version		v0.0.0
 *@date	    	2024-08-23 11:46
 ******************************************/
#ifndef LOGIMPL_HPP__
#define LOGIMPL_HPP__

#include <chrono>
#include <string>
#include <map>
#include <iostream>
#include "LogDef.hpp"

namespace wtlog {
namespace impl {

#define SUFFIX std::string()

class TimeStamp {
public:
    TimeStamp() {
        for(int i = 1; i <= 24; i++) {
			std::string str = i < 10 ? "0" + std::to_string(i) : std::to_string(i);
			m_hours.insert(std::make_pair(i, str));
		}
		for(int i = 1; i <= 60; i++) {
			std::string str = i < 10 ? "0" + std::to_string(i) : std::to_string(i);
			m_minutes.insert(std::make_pair(i, str));
		}
		for(int i = 1; i <= 60; i++) {
			std::string str = i < 10 ? "0" + std::to_string(i) : std::to_string(i);
			m_seconds.insert(std::make_pair(i, str));
        }
        using namespace std::chrono;
        auto now = system_clock::to_time_t(system_clock::now());
        auto local = std::localtime(&now);
        m_hour = local->tm_hour;
        m_minute = local->tm_min;
        m_second = local->tm_sec;
        setHour(m_hour);
		setMinute(m_hour);
		setSecond(m_hour);
	}

	~TimeStamp() = default;

public:
	std::string operator()() {
		using namespace std::chrono;
		auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        setMilliSec(now.count() % 1000);
        auto ctm = now.count() / 1000;
        auto cur = std::localtime(&ctm);
        if(cur->tm_hour != m_hour) {
            m_hour = cur->tm_hour;
            setHour(m_hour);
        }
        if(cur->tm_min != m_minute) {
            m_minute = cur->tm_min;
            setMinute(m_minute);
        }
        if(cur->tm_sec != m_second) {
            m_second = cur->tm_sec;
            setSecond(m_second);
        }
		return m_stampstr;
	}

private:
	void setHour(int h) {
		int pos = 0;
		for(std::size_t i = 0; i < m_hours.at(h).size(); i++) {
			m_stampstr[pos++] = m_hours.at(h).at(i);
		}
	}

	void setMinute(int min) {
		int pos = 3;
		for(std::size_t i = 0; i < m_minutes.at(min).size(); i++) {
			m_stampstr[pos++] = m_minutes.at(min).at(i);
		}
	}

	void setSecond(int sec) {
		int pos = 6;
		for(std::size_t i = 0; i < m_seconds.at(sec).size(); i++) {
			m_stampstr[pos++] = m_seconds.at(sec).at(i);
		}
	}

	void setMilliSec(int millisec) {
		std::string milli = std::to_string(millisec);
		int pos = 9;
		for(std::size_t i = milli.size(); i < 3; i++) {
			m_stampstr[pos++] = '0';
		}
		for(std::size_t i = 0; i < milli.size(); i++) {
			m_stampstr[pos++] = milli[i];
		}
	}

private:
	int m_hour{};
	int m_minute{};
	int m_second{};
	char m_stampstr[13]{ "00:00:00.000" };
	std::map<int, std::string> m_hours{};
	std::map<int, std::string> m_minutes{};
	std::map<int, std::string> m_seconds{};
};

class LogImpl {
public:
	

	
private:
	TimeStamp m_timestamp{};
};

}   // !namespace impl
}	// !namespace wtlog



#endif //!LogImpl.hpp