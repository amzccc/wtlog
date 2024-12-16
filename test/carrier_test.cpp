#include "includes.h"
#include <wtlog/details/log_carrier.h>

using wtlog::details::SimpleCarrier;
using wtlog::details::BuffCarrier;

TEST_CASE("simple carrier test", "[SimpleCarrier]") {
    SimpleCarrier carrier;
    CHECK(carrier.state() == SimpleCarrier::Status::valid);
    std::string msg("log message");
    auto len = carrier.store(msg);
    CHECK(len == msg.size());
    REQUIRE(carrier.state() == SimpleCarrier::Status::ready);
    REQUIRE(carrier.content() == msg);
}

TEST_CASE("simple carrier transfer test", "[SimpleCarrier]") {
    auto carrier = std::make_shared<SimpleCarrier>();
    REQUIRE(carrier != nullptr);
    CHECK(carrier->state() == SimpleCarrier::Status::valid);
    std::string msg("log message");
    auto len = carrier->store(msg);
    CHECK(carrier->state() == SimpleCarrier::Status::ready);
    auto other = carrier->transfer();
    REQUIRE(other != nullptr);
    CHECK(other->state() == SimpleCarrier::Status::ready);
    CHECK(carrier->state() == SimpleCarrier::Status::valid);
    REQUIRE(other->content() == "log message");
    REQUIRE(carrier->empty() == true);
}

TEST_CASE("buffer carrier test", "[BuffCarrier]") {
    BuffCarrier carrier(100);
    CHECK(carrier.state() == BuffCarrier::Status::valid);
    std::string msg("0123456789");
    auto len = carrier.store(msg);
    CHECK(len == msg.size());
    REQUIRE(carrier.state() == BuffCarrier::Status::valid);
    REQUIRE(carrier.content() == msg);
}

TEST_CASE("buffer carrier full test", "[BuffCarrier]") {
    BuffCarrier carrier(100);
    CHECK(carrier.state() == BuffCarrier::Status::valid);
    std::string msg("0123456789");
    wtlog::ui64_t total = 0;
    std::string sum;
    for(int i = 0; i < 10; i++) {
        auto len = carrier.store(msg);
        CHECK(len == msg.size());
        total += len;
        sum += msg;
        if(i != 9) {
            REQUIRE(carrier.state() == BuffCarrier::Status::valid);
        }
        else {
            REQUIRE(carrier.state() == BuffCarrier::Status::ready);
        }
    }
    REQUIRE(carrier.content() == sum);
}

TEST_CASE("buffer carrier overflow test", "[BuffCarrier]") {
    BuffCarrier carrier(9);
    CHECK(carrier.state() == BuffCarrier::Status::valid);
    auto len = carrier.store("0123456789");
    CHECK(len == 9);
    CHECK(carrier.state() == BuffCarrier::Status::ready);
    REQUIRE(carrier.content() == "012345678");
}

TEST_CASE("buffer carrier transfer test", "[BuffCarrier]") {
    auto carrier = std::make_shared<BuffCarrier>(100);
    REQUIRE(carrier != nullptr);
    CHECK(carrier->state() == BuffCarrier::Status::valid);
    std::string msg(100, '1');
    auto len = carrier->store(msg);
    CHECK(len == 100);
    CHECK(carrier->state() == BuffCarrier::Status::ready);
    auto other = carrier->transfer();
    CHECK(other->state() == BuffCarrier::Status::ready);
    CHECK(carrier->state() == BuffCarrier::Status::valid);
    REQUIRE(other->content() == msg);
    REQUIRE(carrier->empty() == true);
}