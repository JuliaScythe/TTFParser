//
// Created by jake on 09/11/2020.
//

#ifndef NEA_HEADTABLE_H
#define NEA_HEADTABLE_H

#include <vector>
#include <cstdint>
#include "Header.h"

class HEADTable {
public:
    HEADTable();

    HEADTable(std::vector<uint8_t> *data, Header header);

    void parse(std::vector<uint8_t> *data, int offset, int length);

    uint16_t flags;
    uint16_t unitsPerEm;
    int16_t xMin;
    int16_t xMax;
    int16_t yMin;
    int16_t yMax;

};


#endif //NEA_HEADTABLE_H
