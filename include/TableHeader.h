//
// Created by jake on 09/11/2020.
//

#ifndef NEA_TABLEHEADER_H
#define NEA_TABLEHEADER_H

#include <string>
#include <vector>
#include <cstdint>

class TableHeader {
public:
    TableHeader();
    TableHeader(std::vector<uint8_t>* data, int offset);
    ~TableHeader() = default;

    std::string tag;
    uint32_t checksum;
    uint32_t tableOffset;
    uint32_t length;
};


#endif //NEA_TABLEHEADER_H
