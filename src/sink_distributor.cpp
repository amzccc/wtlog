#include "wtlog/sinks/sink_distributor.h"
#include <atomic>

using namespace wtlog;

namespace {

static ui64_t seq() {
    static std::atomic<ui64_t> seq{};
    return ++seq;
}

} // !namespace

Pointer<sinks::SinkSplitter> wtlog::sinks::SinkSplitter::instance() {
    static Pointer<SinkSplitter> m_instance(new SinkSplitter);
    return m_instance;
}

ui64_t wtlog::sinks::SinkSplitter::registerSink(wtlog::Pointer<sinks::Sinker> sinker) {
    auto key = seq();
    m_sinks.insert(std::make_pair(key, sinker));
    return key;
}

ui64_t wtlog::sinks::SinkSplitter::unregisterSink(wtlog::Pointer<sinks::Sinker> sinker) {
    for(auto [seq, sk] : m_sinks) {
        if(sk == sinker) {
            m_sinks.erase(seq);
            return seq;
        }
    }
    return 0;
}

void wtlog::sinks::SinkSplitter::distribute(Pointer<details::Carrier> carrier, const std::vector<ui64_t>& sinks_no) {
    for(auto num : sinks_no) {
        auto iter = m_sinks.find(num);
        if(iter != m_sinks.end()) {
            iter->second->collect(carrier);
        }
    }
}
