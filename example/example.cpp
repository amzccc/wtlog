#include <wtlog/wtlog.h>
#include <wtlog/sinks/file_sinker.h>
#include <wtlog/sinks/console_sinker.h>
#include <iostream>

using namespace std;

void loggerDemo();
void asyncLoggerDemo();
void consoleSinkerDemo();
void simpleFileSinkerDemo();
void rotateFileSinkerDemo();
void dailyFileSinkerDemo();
void simpleCarrierDemo();
void buffCarrierDemo();

int main() {
    loggerDemo();
    asyncLoggerDemo();
    consoleSinkerDemo();
    simpleFileSinkerDemo();
    rotateFileSinkerDemo();
    dailyFileSinkerDemo();
    simpleCarrierDemo();
    buffCarrierDemo();

    return 0;
}

void loggerDemo() {
    auto logger = wtlog::logCreate();
    wtlog::logRemove(logger);
}

void asyncLoggerDemo() {
    auto async_logger = wtlog::logCreate<wtlog::AsyncLogger>();
    wtlog::logRemove(async_logger);
}

void consoleSinkerDemo() {
    auto logger = wtlog::logCreate();
    auto sinker = logger->createSinker<wtlog::sinks::ConsoleSinker>();
    logger->detachSinker(sinker);
    wtlog::logRemove(logger);
}

void simpleFileSinkerDemo() {
    auto logger = wtlog::logCreate();
    auto sinker = logger->createSinker<wtlog::sinks::SimpleFileSinker>("name.txt");
    logger->detachSinker(sinker);
    wtlog::logRemove(logger);
}

void rotateFileSinkerDemo() {
    auto logger = wtlog::logCreate();
    // auto sinker = logger->createSinker<wtlog::sinks::RotateFileSinker>();
    // logger->detachSinker(sinker);
    wtlog::logRemove(logger);
}

void dailyFileSinkerDemo() {
    auto logger = wtlog::logCreate();
    // auto sinker = logger->createSinker<wtlog::sinks::DailyFileSinker>();
    // logger->detachSinker(sinker);
    wtlog::logRemove(logger);
}

void simpleCarrierDemo() {
    auto logger = wtlog::logCreate();
    logger->setCarrier<wtlog::details::SimpleCarrier>();
    wtlog::logRemove(logger);
}

void buffCarrierDemo() {
    auto logger = wtlog::logCreate();
    logger->setCarrier<wtlog::details::BuffCarrier>();
    wtlog::logRemove(logger);
}
