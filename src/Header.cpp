//
// Created by jake on 09/11/2020.
//

#include "../include/Header.h"
#include "../include/util.h"
#include "../include/log.h"
#include <iostream>

Header::Header(std::vector<uint8_t>* data) {
    fontType = parseu32(data, 0);
    numTables = parseu16(data, 4);
    // skipping searchRange, entrySelector, rangeShift, they're not relevant.

    tables = std::map<std::string, TableHeader>();

    info("-> Enumerating tables...", "");

    // ok, now time to parse each table
    for (int offset=12; offset < numTables*16+12; offset += 16) {
        auto table = TableHeader(data, offset);
        tables[table.tag] = table;
    }
    // Print each table to the screen, because logging is important!

    for (auto table : tables) {
        info("->-> Found table " + table.first + " @ " + std::to_string(table.second.tableOffset), "");
    }

    // And we're done!
}
