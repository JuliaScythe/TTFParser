//
// Created by jake on 16/11/2020.
//

#include "../include/CMAPTable.h"
#include "../include/util.h"

CMAPTable::CMAPTable() {

}

CMAPTable::CMAPTable(std::vector<uint8_t> *data, Header header) {
    // Look for the table in the header
    TableHeader table = header.tables["cmap"];

    parse(data, table.tableOffset, table.length);
}

void CMAPTable::parse(std::vector<uint8_t> *data, int offset, int length) {
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


}
