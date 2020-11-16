//
// Created by jake on 09/11/2020.
//

#include "../include/TableHeader.h"
#include "../include/util.h"


TableHeader::TableHeader(std::vector<uint8_t>* data, int offset) {
    tag = parseString(data, offset, 4);
    checksum = parseu32(data, offset+4);
    tableOffset = parseu32(data, offset+8);
    length = parseu32(data, offset+12);
}

TableHeader::TableHeader() {

}
