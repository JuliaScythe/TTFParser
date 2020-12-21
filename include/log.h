//
// Created by jake on 14/12/2020.
//

#ifndef NEA_LOG_H
#define NEA_LOG_H

#include <iostream>

void info(const std::string& prompt, const std::string& data);
void error(const std::string& message);

std::string itoh(long val);
#endif //NEA_LOG_H
