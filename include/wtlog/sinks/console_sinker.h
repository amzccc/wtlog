/*********************************************************************************
 * @file		console_sinker.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-29 14:41
 **********************************************************************************/
#ifndef CONSOLE_SINKER_H__
#define CONSOLE_SINKER_H__

#include "sinker.h"
#include <array>

namespace wtlog {
namespace sinks {

class ConsoleSinker : public Sinker,
                      public std::enable_shared_from_this<ConsoleSinker> {
public:
    enum class Color : int {
        black = 30,
        red = 31,
        green = 32,
        yellow = 33,
        blue = 34,
        magenta = 35,
        cyan = 36,
        white = 37,
        light_black = 90,
        light_red = 91,
        light_green = 92,
        light_yellow = 93,
        light_blue = 94,
        light_magenta = 95,
        light_cyan = 96,
        light_white = 97,
    };

public:
    /**
     * @brief 将日志通过标准错误流输出到终端上显示
     */
    void flush() override;

    /**
     * @brief 设置终端上日志显示的文字颜色
     * @param color 设置的颜色
     */
    void setFrontColor(Color color);

    /**
     * @brief 设置终端日志的背景颜色
     * @param color 设置的颜色
     */
    void setBackColor(Color color);

    /**
     * @brief 设置日志文字是否显示为粗体
     * @param is_bold 显示为粗体时置true，否则置false
     */
    void setBold(bool is_bold);

    /**
     * @brief 设置日志文字显示为斜体，有可能终端字体不支持
     * @param is_italic 显示为斜体置true，否则置false
     */
    void setItalic(bool is_italic);

    /**
     * @brief 设置日志文字是否显示下划线
     * @param is_underline 显示下划线置true，否则置false
     */
    void setUnderline(bool is_underline);

    /**
     * @brief 重置终端文字显示属性为默认
     */
    void resetProperty();

private:
    /**
     * @brief 显示粗体的控制符
     * @return 
     */
    std::string bold();

    /**
     * @brief 显示斜体的控制符
     * @return 
     */
    std::string italic();

    /**
     * @brief 显示下划线的控制符
     * @return 
     */
    std::string underline();

    /**
     * @brief 终端文字打印属性
     * @return 
     */
    std::string property();
    
private:
    std::string m_frontcolor{};
    std::string m_backcolor{};
    std::array<bool, 3> m_fonts{ false, false, false };
    static std::vector<std::string> colors;
};

} // !namespace sinks
} // !namespace wtlog


#endif // !CONSOLE_SINKER_H__