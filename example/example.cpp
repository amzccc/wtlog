#include "wtlog/wtlog.h"
#include "wtlog/sinks/file_sinker.h"
#include <iostream>

using namespace std;


int main() {
    auto logger = wtlog::logCreate<wtlog::Logger>();
    logger->attachSinker(std::make_shared<wtlog::sinks::SimpleFileSinker>("example.log"));
    logger->setCarrier(std::make_shared<wtlog::details::BuffCarrier>(4 * 1024));

    return 0;
}