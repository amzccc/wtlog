#include <wtlog/sinks/console_sinker.h>
#include <iostream>

void wtlog::sinks::ConsoleSinker::flush(std::string_view message) {
    std::cerr << property() << message << "\033[0m";
}

void wtlog::sinks::ConsoleSinker::setFrontColor(Color color) {
    m_frontcolor = "\033[" + std::to_string(static_cast<int>(color)) + "m";
}

void wtlog::sinks::ConsoleSinker::setBackColor(Color color) {
    m_backcolor = "\033[" + std::to_string(static_cast<int>(color) + 10) + "m";
}

void wtlog::sinks::ConsoleSinker::setBold(bool is_bold) {
    m_fonts[0] = is_bold;
}

void wtlog::sinks::ConsoleSinker::setItalic(bool is_italic) {
    m_fonts[1] = is_italic;
}

void wtlog::sinks::ConsoleSinker::setUnderline(bool is_underline) {
    m_fonts[2] = is_underline;
}

void wtlog::sinks::ConsoleSinker::resetProperty() {
    m_fonts = {false, false, false};
    m_frontcolor.clear();
    m_backcolor.clear();
}

std::string wtlog::sinks::ConsoleSinker::bold() {
    return "\033[1m";
}

std::string wtlog::sinks::ConsoleSinker::italic() {
    return "\033[3m";
}

std::string wtlog::sinks::ConsoleSinker::underline() {
    return "\033[4m";
}

std::string wtlog::sinks::ConsoleSinker::property() {
    std::string res;
    if(!m_frontcolor.empty()) {
        res += m_frontcolor;
    }
    if(!m_backcolor.empty()) {
        res += m_backcolor;
    }
    if(m_fonts[0]) {
        res += bold();
    }
    if(m_fonts[1]) {
        res += italic();
    }
    if(m_fonts[2]) {
        res += underline();
    }
    return res;
}
