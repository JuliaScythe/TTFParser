//
// Created by jake on 16/11/2020.
//

#include "../include/Glyph.h"
#include "../include/util.h"
#include "../include/PointFlag.h"
#include "../include/Point.h"

Glyph::Glyph(std::vector<uint8_t> *data, uint32_t offset, uint32_t length) {
    parse(data, offset, length);
}

Glyph::Glyph() {

}

void Glyph::parse(std::vector<uint8_t> *data, uint32_t offset, uint32_t length) {
    // The number of contours in the glyph
    numberOfContours = parse16(data, offset);
    xMin = parse16(data, offset+2);
    yMin = parse16(data, offset+4);
    xMax = parse16(data, offset+6);
    yMax = parse16(data, offset+8);

    if (numberOfContours < 0) {
        // Compound glyph, out of the scope of the program
        abort();
    }

    endPointsOfContours = {};
    for (int i=0; i < numberOfContours; i++) {
        endPointsOfContours.push_back(parseu16(data, offset+10+(i*2)));
    }

    uint32_t instructionLengthOffset = offset + 10 + numberOfContours*2;

    instructionLength = parseu16(data, instructionLengthOffset);

    // Skip the instructions, they're not used here
    uint32_t dataOffset = instructionLengthOffset + instructionLength;
    uint32_t currentOffset = dataOffset;
    uint32_t remainingLength = length - (dataOffset - offset);

    uint16_t totalPoints = endPointsOfContours.back();
    uint16_t numberOfPoints = 0;
    uint16_t numberOfXValues = 0;
    uint16_t numberOfYValues = 0;
    std::vector<PointFlag> flags = {};
    std::vector<Point> points = {};

    // THE BIGGER ALGORITHM

    // Attempt to read each flag
    while (numberOfPoints < totalPoints) {
        flags.emplace_back(PointFlag(data, currentOffset)); // kinda cool that we can just pass stuff like this
        numberOfPoints += 1;
        currentOffset++;
    } // TODO: handle repeating?

    currentOffset -= 1;

    uint16_t lastX = 0;
    uint16_t lastY = 0;


    // Parse the X values
    for(auto flag : flags) {
        int16_t xDelta;
        if(flag.xShortVector) {
            currentOffset++;
            bool sign = flag.xSame;
            // Read the single byte of the vector
            xDelta = parseu8(data, currentOffset);
            if(!sign) { // If the sign is negative, invert the delta
                xDelta = -xDelta;
            }

        } else {
            if(!flag.xSame) {
                currentOffset +=2;
            }
            xDelta = (!flag.xSame) ? parse16(data, currentOffset) : lastX;

        }

        // Create the point
        points.emplace_back(xDelta, 0, flag); // DeltaY will be added below.
        // Update lastX
        lastX = xDelta;
    }
    // This might just be valid!

/*
    // Parse the Y values
    for(auto flag : flags) {
        int16_t yDelta;
        if(flag.yShortVector) {
            currentOffset++;
            bool sign = flag.ySame;
            // Read the single byte of the vector
            yDelta = parseu8(data, currentOffset);
            if(!sign) { // If the sign is negative, invert the delta
                yDelta = -yDelta;
            }

        } else {
            if(!flag.ySame) {
                currentOffset +=2;
            }
            yDelta = (!flag.ySame) ? parse16(data, currentOffset) : lastY;

        }

        // Create the point
        // FIXME: points.emplace_back(yDelta, 0, flag); // DeltaY will be added below.
        // Update lastX
        lastX = xDelta;
    }
*/
}
