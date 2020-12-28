//
// Created by jake on 16/11/2020.
//

#include "../include/Glyph.h"
#include "../include/util.h"
#include "../include/log.h"

Glyph::Glyph(std::vector<uint8_t> *data, uint32_t offset, uint32_t length) : points() {
    parse(data, offset, length);
}

Glyph::Glyph() {

}

void Glyph::parse(std::vector<uint8_t> *data, uint32_t offset, uint32_t length) {
    // The number of contours in the glyph
    numberOfContours = parseu16(data, offset);
    xMin = parseu16(data, offset+2);
    yMin = parseu16(data, offset+4);
    xMax = parseu16(data, offset+6);
    yMax = parseu16(data, offset+8);

    info("numberOfContours: ", std::to_string(numberOfContours));

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

    uint16_t totalPoints = endPointsOfContours.back() + 1;
    uint16_t numberOfPoints = 0;
    uint16_t numberOfXValues = 0;
    uint16_t numberOfYValues = 0;
    std::vector<PointFlag> flags = {};
    std::vector<int16_t> xDeltas = {};
    std::vector<int16_t> yDeltas = {};

    // THE BIGGER ALGORITHM

    // Attempt to read each flag
	currentOffset+=2;
    while (numberOfPoints < totalPoints) {
        flags.emplace_back(PointFlag(data, currentOffset)); // kinda cool that we can just pass stuff like this
        numberOfPoints += 1;
        currentOffset++;
    } // TODO: handle repeating?

    currentOffset -= 1;

    int16_t lastX = 0;
    int16_t lastY = 0;


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
				currentOffset +=1;
				xDelta = parse16(data, currentOffset);
				currentOffset +=1;
			}
            else
				xDelta = 0;//lastX;
		}

        xDeltas.emplace_back(xDelta);
        // Update lastX
        lastX = xDelta;
    }
    // This might just be valid!

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
				currentOffset +=1;
				yDelta = parse16(data, currentOffset);
				currentOffset +=1;
			}
			else
				yDelta = 0;//lastY;
        }

        yDeltas.emplace_back(yDelta);
        // Update lastY
        lastY = yDelta;
    }

    for(int i = 0; i < xDeltas.size(); i++) {
        points.emplace_back(xDeltas[i], yDeltas[i], flags[i]);
    }


}
