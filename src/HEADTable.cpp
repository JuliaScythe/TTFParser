//
// Created by jake on 09/11/2020.
//

#include "../include/HEADTable.h"
#include "../include/util.h"
#include "../include/log.h"
#include <iostream>


void HEADTable::parse(std::vector<uint8_t> *data, int offset, int length) {

    info("⟶ Parsing HEAD table @ ", "");

    // Offset +16: flag word
    flags = parseu16(data, offset+16);

    // Offset +18: unitsPerEm
    unitsPerEm = parseu16(data, offset+18);

    // Offset +36: xMin
    xMin = parse16(data, offset+36);
    yMin = parse16(data, offset+38);

    xMax = parse16(data, offset+40);
    yMax = parse16(data, offset+42);

    indexToLocFormat = (bool) parseu16(data, offset+50);
    // Most of the rest of HEAD is kinda irrelevant, checksum, various other properties don't really matter
}

HEADTable::HEADTable() {

}

HEADTable::HEADTable(std::vector<uint8_t> *data, Header header) {
    // Look for the table in the header
    TableHeader table = header.tables["head"];

    parse(data, table.tableOffset, table.length);
}
