/******************************************
 *@file		    LogImpl.hpp
 *@brief		Logger的实现类
 *@author		cwt
 *@version		v0.0.0
 *@date	    	2024-08-23 11:46
 ******************************************/
#ifndef LOGIMPL_HPP__
#define LOGIMPL_HPP__

#include <memory>
#include "LogConfig.hpp"
#include "LogUtils.hpp"

namespace wtlog {
namespace impl {

class LogImpl {
public:
	

	
private:
	utils::Clock m_clock{};
	std::shared_ptr<config::Configer> m_config{ std::make_shared<config::Configer>() };
};

}   // !namespace impl
}	// !namespace wtlog



#endif //!LogImpl.hpp