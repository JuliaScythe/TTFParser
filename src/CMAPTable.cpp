//
// Created by jake on 16/11/2020.
//

#include "../include/CMAPTable.h"
#include "../include/util.h"

CMAPTable::CMAPTable() {

}

CMAPTable::CMAPTable(std::vector<uint8_t> *data, Header header, char characterToGet) {
    // Look for the table in the header
    TableHeader table = header.tables["cmap"];

    parse(data, table.tableOffset, table.length, characterToGet);
}

void CMAPTable::parse(std::vector<uint8_t> *data, int offset, int length, char characterToGet) {
    // Parsing the cmap table is a bit more complex, there are multiple possible formats it could be in.
    uint16_t numSubtables = parseu16(data, offset+2);

    std::vector<uint16_t> platformIds = {};
    std::vector<uint16_t> platformSpecificIds = {};
    std::vector<uint32_t> offsets = {};

    // Enumerate the subtables
    for (int i=offset+4; i<offset+4+(8*numSubtables); i+=8) {
        platformIds.push_back(parseu16(data, i));
        platformSpecificIds.push_back(parseu16(data, i+2));
        offsets.push_back(parseu32(data, i+4));
    }

    uint16_t subOffset = 0;

    // Look for a table with type 0/3 or 0/4 (unicode)
    for (int i=0; i<numSubtables; i++) {
        if(
                platformIds[i] == 0 &&
                (platformSpecificIds[i] == 4 || platformSpecificIds[i] == 3)
        ) {
            subOffset = offsets[i];
            break;
        }
    }

    // If we didn't find a suitable subtable
    if (subOffset == 0) {
        std::abort(); // TODO: proper error handling
    }



    // So now we know which subtable to use, and we have it's offset
    subOffset = subOffset + offset;
    // Find the subtable format
    uint16_t subtableFormat = parseu16(data, subOffset);

    // Right now, my font only has a type 4 table
    // TODO: Add support for more subtable types
    if(subtableFormat == 4) {
        parseSubtableType4(data, subOffset, characterToGet);
    } else {
        std::abort(); // TODO: add error handling
    }
}

void CMAPTable::parseSubtableType4(std::vector<uint8_t> *data, int offset, char characterToMap) {
    uint16_t subtableLength = parseu16(data, offset+2);
    uint16_t languageCode = parseu16(data, offset+4);
    uint16_t segCount = parseu16(data, offset+6) / 2; // This is inexplicably doubled in the file, idk why

    // Next few bytes are weird old optimisation stuff, we can safely ignore them

    std::vector<uint16_t> endCodes = {};
    for (int i=offset+14; i<offset+14+(2*segCount); i+=2) {
        endCodes.push_back(parseu16(data, i));
    }
    std::vector<uint16_t> startCodes = {};
    for (int i=offset+16+(2*segCount); i<offset+16+(4*segCount); i+=2) {
        startCodes.push_back(parseu16(data, i));
    }

    std::vector<uint16_t> idDeltas = {};
    for (int i=offset+16+(4*segCount); i<offset+16+(6*segCount); i+=2) {
        idDeltas.push_back(parseu16(data, i));
    }
    std::vector<uint16_t> idRangeOffsets = {};
    for (int i=offset+16+(6*segCount); i<offset+16+(8*segCount); i+=2) {
        idRangeOffsets.push_back(parseu16(data, i));
    }

    uint16_t startCode;
    uint16_t endCode;
    uint16_t idDelta;
    uint16_t idRangeOffset;

    // Search for the character in the data structures
    for (int i=0; i<endCodes.size(); i++) {
        if (endCodes[i] >= characterToMap) {
            if(startCodes[i] <= characterToMap) {
                // We've found our character!
                startCode = startCodes[i];
                endCode = endCodes[i];
                idDelta = idDeltas[i];
                idRangeOffset = idRangeOffsets[i];
                break;
            } else {
                std::abort(); // oh no
            }
        }
    }

    if (idRangeOffset != 0) {
        std::abort();
    }

    glyphIndex = idDelta + characterToMap;

}
