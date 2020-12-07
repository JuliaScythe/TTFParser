//
// Created by kernel on 16/11/2020.
//

#ifndef NEA_GLYPH_H
#define NEA_GLYPH_H

#include <cstdint>
#include <vector>

class Glyph {
public:
    Glyph();
    Glyph(std::vector<uint8_t>* data, uint32_t offset, uint32_t length);

    void parse(std::vector<uint8_t>* data, uint32_t offset, uint32_t length);

    int16_t numberOfContours;
    int16_t xMin;
    int16_t yMin;
    int16_t xMax;
    int16_t yMax;

    std::vector<uint16_t> endPointsOfContours;
    uint16_t instructionLength;


};


#endif //NEA_GLYPH_H
