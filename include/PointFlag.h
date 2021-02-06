//
// Created by jake on 25/11/2020.
//

#ifndef NEA_POINTFLAG_H
#define NEA_POINTFLAG_H

#include <vector>
#include <cstdint>

class PointFlag {
public:
    PointFlag();
    explicit PointFlag(uint8_t flagByte);
    PointFlag(std::vector<uint8_t>* data, uint32_t offset);

    bool onCurvePoint;
    bool xShortVector;
    bool yShortVector;
    bool repeat;
    uint8_t repeatCount;
    bool xSame;
    bool ySame;
    bool overlapSimple;

    uint8_t flagByte;
};


#endif //NEA_POINTFLAG_H
