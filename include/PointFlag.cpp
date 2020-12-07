//
// Created by jake on 25/11/2020.
//

#include "PointFlag.h"

PointFlag::PointFlag(std::vector<uint8_t> *data, uint32_t offset) {
    flagByte = data->at(offset);

    onCurvePoint = flagByte & 0b00000001;
    xShortVector = flagByte & 0b00000010;
    yShortVector = flagByte & 0b00000100;
    repeat       = flagByte & 0b00001000;
    xSame        = flagByte & 0b00010000;
    ySame        = flagByte & 0b00100000;
    overlapSimple= flagByte & 0b01000000;
    // reserved  = flagByte & 0b10000000;

    if (repeat) {
        repeatCount = data->at(offset+1);
    } else {
        repeatCount = 0;
    }
}

PointFlag::PointFlag() = default;
