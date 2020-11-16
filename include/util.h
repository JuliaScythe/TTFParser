//
// Created by jake on 09/11/2020.
//

#ifndef NEA_UTIL_H
#define NEA_UTIL_H

#include <cstdint>
#include <vector>
#include <string>

uint32_t parseu32(std::vector<uint8_t>* data, int offset);
uint16_t parseu16(std::vector<uint8_t>* data, int offset);
uint8_t parseu8(std::vector<uint8_t>* data, int offset);

int16_t parse16(std::vector<uint8_t>* data, int offset);

std::string parseString(std::vector<uint8_t>* data, int offset, int length);

#endif //NEA_UTIL_H
