#include <wtlog/sinks/sink_distributor.h>
#include <algorithm>


using namespace wtlog;

void wtlog::sinks::SinkDistributor::registerSink(wtlog::Pointer<sinks::Sinker> sinker) {
    m_sinks.emplace_back(sinker);
    m_mask.emplace(sinker->identifier(), true);
}

void wtlog::sinks::SinkDistributor::unregisterSink(wtlog::Pointer<sinks::Sinker> sinker) {
    m_sinks.erase(std::remove(m_sinks.begin(), m_sinks.end(), sinker));
    m_mask.erase(sinker->identifier());
}

void wtlog::sinks::SinkDistributor::distribute(Pointer<details::Carrier> carrier) {
    return doDistribute(carrier);
}

void wtlog::sinks::SinkDistributor::mask(Pointer<Sinker> sinker, bool mask) {
    auto iter = m_mask.find(sinker->identifier());
    if(iter == m_mask.end()) {
        return;
    }
    iter->second = mask;
}

void wtlog::sinks::SinkDistributor::doDistribute(const Pointer<details::Carrier>& carrier) {
    for(auto& sinker : m_sinks) {
        if(m_mask.at(sinker->identifier())) {
            sinker->collect(carrier);
        }
    }
}
