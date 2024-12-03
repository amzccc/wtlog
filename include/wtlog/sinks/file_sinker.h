/*********************************************************************************
 * @file		file_sinker.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-12-02 10:17
 **********************************************************************************/
#ifndef FILE_SINKER_H__
#define FILE_SINKER_H__

#include "sinker.h"
#include <fstream>
#include <memory>

namespace wtlog {
namespace sinks {
class SimpleFileSinker : public Sinker,
                         public std::enable_shared_from_this<SimpleFileSinker> {
public:
    explicit SimpleFileSinker(const std::string& filepath);

    ~SimpleFileSinker();

public:
    /**
     * @brief 将日志直接刷新到磁盘
     */
    void flush() override;

private:
    std::ofstream m_out{};
};

} // !namespace sinker
} // !namespace wtlog

#endif // !FILE_SINKER_H__