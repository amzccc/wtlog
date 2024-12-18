#include <wtlog/sinks/sinker.h>
#include <atomic>

namespace {
using namespace wtlog;

static ui64_t seq() {
    static std::atomic<ui64_t> seq{};
    return ++seq;
}

} // namespace

wtlog::sinks::SinkIdentifier::SinkIdentifier() : m_id(seq()) {}

wtlog::sinks::SinkIdentifier::SinkIdentifier(SinkIdentifier&& other) : m_id(other.m_id) {}

ui64_t wtlog::sinks::SinkIdentifier::identifier() const {
    return m_id;
}

bool wtlog::sinks::SinkIdentifier::operator==(const SinkIdentifier& other) {
    return m_id == other.m_id;
}
