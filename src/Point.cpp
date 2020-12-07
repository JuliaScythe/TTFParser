//
// Created by jake on 04/12/2020.
//

#include "../include/Point.h"

Point::Point() {
    deltaX = 0;
    deltaY = 0;
}

Point::Point(int16_t deltaX, int16_t deltaY, PointFlag flag) : deltaX(deltaX), deltaY(deltaY), flag(flag) {}
