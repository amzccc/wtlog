#include "includes.h"
#include <iostream>
#include <exception>
using namespace std;


int main(int argc, char** argv) {
    Catch::Session session;
    int returncode = session.applyCommandLine(argc, argv);
    if(returncode != 0) {
        return returncode;
    }
    // auto& config = session.config();

    try {
        session.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}