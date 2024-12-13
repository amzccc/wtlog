#include "includes.h"
#include <wtlog/details/log_utils.h>

using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;
using wtlog::utils::Clock;

std::string timestamp() {
    auto timepoint = std::chrono::duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    time_t utc = std::chrono::duration_cast<seconds>(timepoint).count();
    int millisec = timepoint.count() % 1000;
    std::tm* local = std::localtime(&utc);
    std::string timestamp;
    return timestamp.append(std::to_string(local->tm_year + 1900))
        .append(local->tm_mon + 1 < 10 ? "-0" : "-").append(std::to_string(local->tm_mon + 1))
        .append(local->tm_mday < 10 ? "-0" : "-").append(std::to_string(local->tm_mday))
        .append(local->tm_hour < 10 ? " 0" : " ").append(std::to_string(local->tm_hour))
        .append(local->tm_min < 10 ? ":0" : ":").append(std::to_string(local->tm_min))
        .append(local->tm_sec < 10 ? ":0" : ":").append(std::to_string(local->tm_sec))
        .append(millisec < 10 ? ".00" : millisec < 100 ? ".0" : ".").append(std::to_string(millisec));
}

TEST_CASE("precision millisecond", "[Clock]") {
    wtlog::utils::Clock clocker;
    clocker.setPrecision(Clock::Unit::millisecond);

    REQUIRE(clocker.timestamp() == timestamp());
}

TEST_CASE("precision second", "[Clock]") {
    wtlog::utils::Clock clocker;
    clocker.setPrecision(Clock::Unit::second);
    
    REQUIRE(clocker.timestamp() == timestamp().substr(0, 19));
}

TEST_CASE("precision minute", "[Clock]") {
    wtlog::utils::Clock clocker;
    clocker.setPrecision(Clock::Unit::minute);

    REQUIRE(clocker.timestamp() == timestamp().substr(0, 16));
}

TEST_CASE("precision hour", "[Clock]") {
    wtlog::utils::Clock clocker;
    clocker.setPrecision(Clock::Unit::hour);
    
    REQUIRE(clocker.timestamp() == timestamp().substr(0, 13));
}

TEST_CASE("precision day", "[Clock]") {
    wtlog::utils::Clock clocker;
    clocker.setPrecision(Clock::Unit::day);

    REQUIRE(clocker.timestamp() == timestamp().substr(0, 10));
}