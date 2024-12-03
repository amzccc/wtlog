/*********************************************************************************
 * @file		log_carrier.h
 * @brief		
 * @author		cwt
 * @version		v0.0.0
 * @date		2024-11-19 22:27
 **********************************************************************************/
#ifndef LOG_CARRIER_H__
#define LOG_CARRIER_H__

#include "log_typedef.h"
#include <string>
#include <string_view>
#include <vector>

namespace wtlog {
namespace details {
/**
 * @brief Carrier作为消息传递的载体媒介，提供缓存和读取日志数据的接口。
 */
class Carrier {
public:
    enum class Status : char {
        valid,
        ready
    };

    virtual ~Carrier() = default;

public:
    /**
     * @brief 返回日志传输对象的状态，返回`Carrier::Staus::ready`表明当前的传输对象已经准备好传输了
     * @return 
     */
    virtual Status state() const = 0;

    /**
     * @brief 获取实际的日志内容
     * @return 
     */
    virtual std::string_view content() = 0;

    /**
     * @brief 装载日志内容
     * @param msg 待装载的日志记录
     * @return 返回装载的日志记录大小
     */
    virtual ui64_t store(std::string_view msg) = 0;

    /**
     * @brief 将日志移交给一个新生成的对象，并将其返回出去，当前对象会失去日志内容的所有权并初始化内部状态。
     * @return 新生成的记录了日志信息的传输对象
     */
    virtual Pointer<Carrier> transfer() = 0;

    /**
     * @brief 日志传输对象是否为空
     * @return 
     */
    virtual bool empty() = 0;
};

/**
 * @brief 单条日志数据的传递
 */
class SimpleCarrier : public Carrier {
public:
    SimpleCarrier() = default;

    explicit SimpleCarrier(const std::string& msg);

    SimpleCarrier(const SimpleCarrier&) = default;

    SimpleCarrier(SimpleCarrier&& other) = default;

    ~SimpleCarrier() = default;

public:
    SimpleCarrier& operator=(const SimpleCarrier & other);

    SimpleCarrier& operator=(SimpleCarrier&& other);

    /**
     * @brief 获取实际的日志内容
     * @return 
     */
    std::string_view content() override;

    /**
     * @brief 装载日志内容
     * @param msg 待装载的日志记录
     * @return 返回装载的日志记录大小
     */
    ui64_t store(std::string_view msg) override;

    /**
     * @brief 返回日志传输对象的状态，返回`Carrier::Staus::ready`表明当前的传输对象已经准备好传输了
     * @return 
     */
    Status state() const override;

    /**
     * @brief 将日志移交给一个新生成的对象，并将其返回出去，当前对象会失去日志内容的所有权并初始化内部状态。
     * @return 新生成的记录了日志信息的传输对象
     */
    Pointer<Carrier> transfer() override;

    /**
     * @brief 日志传输对象是否为空
     * @return 
     */
    bool empty() override;

private:
    std::string m_content{};
};

/**
 * @brief 多条日志的传递，通过缓存池缓存多条日志，构造函数入参需要指定缓存池的大小
 */
class BuffCarrier : public Carrier {
public:
    explicit BuffCarrier(ui64_t size);

    BuffCarrier(const BuffCarrier& other);

    BuffCarrier(BuffCarrier&& other);

    ~BuffCarrier();
    
public:
    BuffCarrier& operator=(const BuffCarrier& other);

    BuffCarrier& operator=(BuffCarrier&& other);

    /**
     * @brief 获取实际的日志内容
     * @return 
     */
    std::string_view content() override;

    /**
     * @brief 装载日志内容
     * @param msg 待装载的日志记录
     * @return 返回装载的日志记录大小
     */
    ui64_t store(std::string_view msg) override;

    /**
     * @brief 返回日志传输对象的状态，返回`Carrier::Staus::ready`表明当前的传输对象已经准备好传输了
     * @return 
     */
    Status state() const override;

    /**
     * @brief 将日志移交给一个新生成的对象，并将其返回出去，当前对象会失去日志内容的所有权并初始化内部状态。
     * @return 新生成的记录了日志信息的传输对象
     */
    Pointer<Carrier> transfer() override;

    /**
     * @brief 日志传输对象是否为空
     * @return 
     */
    bool empty() override;

public:
    /**
     * @brief 重置缓冲区的大小
     * @param size 
     */
    void resizeBuffer(ui64_t size);

    /**
     * @brief 交换两个传输对象
     * @param lval 
     * @param rval 
     */
    void swap(BuffCarrier& other);

private:
    char* m_buff;
    ui64_t m_size;
    ui64_t m_tail{ 0 };
};


}   // !namespace details
}   // !namespace wtlog

#endif // !LOG_CARRIER_H__