//
// Created by jake on 09/11/2020.
//

#ifndef NEA_HEADER_H
#define NEA_HEADER_H
#include <vector>
#include <map>
#include <cstdint>
#include <string>
#include "TableHeader.h"

class Header {

public:
    Header() = default;
    Header(std::vector<uint8_t>* data);

    ~Header() = default;

    std::map<std::string, TableHeader> tables;
private:
    uint32_t fontType;
    uint16_t numTables;
};


#endif //NEA_HEADER_H
