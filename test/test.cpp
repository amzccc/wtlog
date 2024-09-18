#include <iostream>
#include <string>
#include "wtlog/LogConfig.hpp"

using namespace std;
using namespace wtlog;



string loadConfigTest() {
    LogConfig logConfig;
    if(logConfig.load("test.toml") != Status::success) {
        return "failure";
    }
    return "pass";
}


int main(int argc, char** argv) {
    

    return 0;
}