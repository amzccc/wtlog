/******************************************
 *@file		    wtlog.hpp
 *@brief		wtlog接口头文件
 *@author		cwt
 *@version		v0.0.0
 *@date		    2024-08-23 11:25
 ******************************************/
#ifndef LOGGERPLUSPLUS_HPP__
#define LOGGERPLUSPLUS_HPP__

#include "impl/LogImpl.hpp"


namespace wtlog {

class Logger {
public:
    

private:
    impl::LogImpl m_impl;
};

}   // !namespace wtlog


#endif //!LOGGERPLUSPLUS_hpp