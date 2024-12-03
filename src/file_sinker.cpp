#include "wtlog/sinks/file_sinker.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

wtlog::sinks::SimpleFileSinker::SimpleFileSinker(const std::string& filename) : m_out(filename, std::ios::app){ }

wtlog::sinks::SimpleFileSinker::~SimpleFileSinker() {
    if(m_out.is_open()) {
        m_out.flush();
        m_out.close();
    }
}

void wtlog::sinks::SimpleFileSinker::flush() {
    if(!m_out.is_open()) {
        return;
    }
    m_out << m_carrier->content();
}