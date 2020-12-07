//
// Created by jake on 04/12/2020.
//

#ifndef NEA_POINT_H
#define NEA_POINT_H


#include "PointFlag.h"

class Point {
public:
    Point();
    Point(int16_t deltaX, int16_t deltaY, PointFlag flag);

    int16_t deltaX;
    int16_t deltaY;

    PointFlag flag;
};


#endif //NEA_POINT_H
