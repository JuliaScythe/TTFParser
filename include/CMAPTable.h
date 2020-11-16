//
// Created by jake on 16/11/2020.
//

#ifndef NEA_CMAPTABLE_H
#define NEA_CMAPTABLE_H

#include <vector>
#include <cstdint>
#include "Header.h"

class CMAPTable {
public:
    CMAPTable();

    CMAPTable(std::vector<uint8_t> *data, Header header);

    void parse(std::vector<uint8_t> *data, int offset, int length);
};


#endif //NEA_CMAPTABLE_H
