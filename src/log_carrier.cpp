#include <wtlog/details/log_carrier.h>
#include <cstring>

wtlog::details::SimpleCarrier::SimpleCarrier(const std::string& msg) : m_content(msg) {}

wtlog::details::SimpleCarrier& wtlog::details::SimpleCarrier::operator=(
    const SimpleCarrier& other) {
    m_content = other.m_content;
    return *this;
}

wtlog::details::SimpleCarrier& wtlog::details::SimpleCarrier::operator=(SimpleCarrier&& other) {
    m_content = std::move(other.m_content);
    return *this;
}

std::string_view wtlog::details::SimpleCarrier::content() {
    return m_content;
}

wtlog::ui64_t wtlog::details::SimpleCarrier::store(std::string_view msg) {
    m_content = msg;
    return msg.size();
}

wtlog::details::Carrier::Status wtlog::details::SimpleCarrier::state() const {
    return m_content.empty() ? Status::valid : Status::ready;
}

wtlog::Pointer<wtlog::details::Carrier> wtlog::details::SimpleCarrier::transfer() {
    Pointer<SimpleCarrier> other = std::make_shared<SimpleCarrier>(*this);
    m_content.clear();
    return other;
}

bool wtlog::details::SimpleCarrier::empty() {
    return m_content.empty();
}

wtlog::details::BuffCarrier::BuffCarrier(ui64_t size) : m_buff{new char[size]{}}, m_size(size) {}

wtlog::details::BuffCarrier::BuffCarrier(const BuffCarrier& other)
    : m_buff(new char[other.m_size]),
      m_size(other.m_size),
      m_tail(other.m_tail) {
    std::memcpy(m_buff, other.m_buff, other.m_size);
}

wtlog::details::BuffCarrier::BuffCarrier(BuffCarrier&& other)
    : m_buff(other.m_buff),
      m_size(other.m_size),
      m_tail(other.m_tail) {
    other.m_buff = nullptr;
}

wtlog::details::BuffCarrier::~BuffCarrier() {
    delete[] m_buff;
}

wtlog::details::BuffCarrier& wtlog::details::BuffCarrier::operator=(const BuffCarrier& other) {
    if(m_size < other.m_size) {
        delete[] m_buff;
        m_buff = new char[other.m_size]{};
        m_size = other.m_size;
    }
    std::memcpy(m_buff, other.m_buff, other.m_size);
    m_tail = other.m_tail;
    return *this;
}

wtlog::details::BuffCarrier& wtlog::details::BuffCarrier::operator=(BuffCarrier&& other) {
    std::swap(m_buff, other.m_buff);
    m_size = other.m_size;
    m_tail = other.m_tail;
    return *this;
}

std::string_view wtlog::details::BuffCarrier::content() {
    return std::string_view(m_buff, m_tail);
}

wtlog::ui64_t wtlog::details::BuffCarrier::store(std::string_view msg) {
    ui64_t len = msg.size() + m_tail > m_size ? m_size - m_tail : msg.size();
    std::memcpy(m_buff + m_tail, msg.data(), len);
    m_tail += len;
    return len;
}

wtlog::details::Carrier::Status wtlog::details::BuffCarrier::state() const {
    return m_tail == m_size ? Status::ready : Status::valid;
}

wtlog::Pointer<wtlog::details::Carrier> wtlog::details::BuffCarrier::transfer() {
    Pointer<BuffCarrier> other = std::make_shared<BuffCarrier>(m_size);
    swap(*other);
    return other;
}

bool wtlog::details::BuffCarrier::empty() {
    return m_tail == 0;
}

void wtlog::details::BuffCarrier::resizeBuffer(ui64_t size) {
    char* tmp = m_buff;
    m_buff = new char[size]{};
    if(size >= m_tail) {
    std:
        memcpy(m_buff, tmp, m_tail);
    } else {
        std::memcpy(m_buff, tmp, size - 1);
    }
    delete[] tmp;
}

void wtlog::details::BuffCarrier::swap(BuffCarrier& other) {
    std::swap(m_buff, other.m_buff);
    std::swap(m_size, other.m_size);
    std::swap(m_tail, other.m_tail);
}