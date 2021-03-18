//
// Created by jake on 09/11/2020.
//

#include "../include/util.h"
#include <cstring>

uint32_t parseu32(std::vector<uint8_t>* data, int offset) {

    uint32_t val = (uint32_t) data->at(offset);
    val <<= 8;
    val += data->at(offset+1);
    val <<= 8;
    val += data->at(offset+2);
    val <<= 8;
    val += data->at(offset+3);

    return val;
}

uint16_t parseu16(std::vector<uint8_t>* data, int offset) {
    uint16_t val = (uint16_t) data->at(offset);
    val <<= 8;
    val += data->at(offset+1);

    return val;
}

std::string parseString(std::vector<uint8_t> *data, int offset, int length) {
    std::string result = std::string("");
    for(int i=offset; i<offset+length; i++) {
        result += (char) data->at(i);
    }
    return result;
}

uint8_t parseu8(std::vector<uint8_t> *data, int offset) {
    return data->at(offset);
}

int16_t parse16(std::vector<uint8_t> *data, int offset) {

	auto higher = data->at(offset);
	auto lower = data->at(offset+1);


	auto val = (uint16_t) data->at(offset);
    val <<= 8;
    val |= (uint16_t) data->at(offset+1);

    int16_t result;
    std::memcpy(&result, &val, 2); // Copy two bytes of memory from val -> result
    // We need to do this because leftshifting a signed integer is UB and c++ doesn't want to reinterpret it

    return result;
}
