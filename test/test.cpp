#include <iostream>
#include <string>
#include "wtlog/wtlog.h"
#include "wtlog/sinks/file_sinker.h"
#include "wtlog/sinks/console_sinker.h"

using namespace std;
using namespace wtlog;

// simple file sinker
string test1(Pointer<Logger> logger) {
    Pointer<sinks::SimpleFileSinker> simple = std::make_shared<sinks::SimpleFileSinker>("simplefile_test.log");
    logger->attachSinker(simple);
    error("errorcode{}", 42);
    logger->detachSinker(simple);
    return "pass";
}

// console sinker front color
string test2(Pointer<Logger> logger) {
    Pointer<sinks::ConsoleSinker> console = std::make_shared<sinks::ConsoleSinker>();
    logger->attachSinker(console);
    console->setFrontColor(sinks::ConsoleSinker::Color::green);
    error("errorcode{}", 42);
    logger->detachSinker(console);
    return "pass";
}

// console sinker back color
string test3(Pointer<Logger> logger) {
    Pointer<sinks::ConsoleSinker> console = std::make_shared<sinks::ConsoleSinker>();
    logger->attachSinker(console);
    console->setBackColor(sinks::ConsoleSinker::Color::green);
    error("errorcode{}", 42);
    logger->detachSinker(console);
    return "pass";
}

// console sinker bold color
string test4(Pointer<Logger> logger) {
    Pointer<sinks::ConsoleSinker> console = std::make_shared<sinks::ConsoleSinker>();
    logger->attachSinker(console);
    console->setBold(true);
    error("errorcode{}", 42);
    logger->detachSinker(console);
    return "pass";
}

// console sinker italic color
string test5(Pointer<Logger> logger) {
    Pointer<sinks::ConsoleSinker> console = std::make_shared<sinks::ConsoleSinker>();
    logger->attachSinker(console);
    console->setItalic(true);
    error("errorcode{}", 42);
    logger->detachSinker(console);
    return "pass";
}

// concole sinker underline color
string test6(Pointer<Logger> logger) {
    Pointer<sinks::ConsoleSinker> console = std::make_shared<sinks::ConsoleSinker>();
    logger->attachSinker(console);
    console->setUnderline(true);
    error("errorcode{}", 42);
    logger->detachSinker(console);
    return "pass";
}

// buff carrier 
string test7(Pointer<Logger> logger) {
    Pointer<sinks::SimpleFileSinker> simple = std::make_shared<sinks::SimpleFileSinker>("simplefile_test.log");
    logger->setCarrier(std::make_shared<details::BuffCarrier>(4096));
    logger->attachSinker(simple);
    for(int i = 0; i < 100; i++) {
        error("errorcode{}", 42);
    }
    logger->detachSinker(simple);
    return "pass";
}


int main() {
    {
        auto logger = logCreate<Logger>();
        logger->attachSinker(std::make_shared<wtlog::sinks::SimpleFileSinker>("example.log"));
        logger->setCarrier(std::make_shared<wtlog::details::BuffCarrier>(4 * 1024));
        auto start = chrono::steady_clock::now();
        for(int i = 0; i < 10; i++) {
            wtlog::error("use wtlog print log, sinker is {}, carrier is {}", "simplefile", "buffcarrier");
        }
        auto end = chrono::steady_clock::now();
        cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
    }

    return 0;
}