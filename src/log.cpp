//
// Created by jake on 14/12/2020.
//

#include "../include/log.h"
#include <sstream>

void error(const std::string& message) {
    std::cerr << message << std::endl;
}

void info(const std::string& prompt, const std::string& data) {
    std::cout << prompt << data << std::endl;
}

std::string itoh(long val) {
    std::stringstream s;
    s << std::hex << val;
    std::string result( s.str() );
    return result;
}
