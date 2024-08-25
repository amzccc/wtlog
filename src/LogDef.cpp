#include "wtlog/LogDef.hpp"

using namespace wtlog;


LogConfig& wtlog::LogConfig::instance() {
    static LogConfig instance;
    return instance;
}