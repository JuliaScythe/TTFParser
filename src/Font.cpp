//
// Created by jake on 12/10/2020.
//
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../include/Font.h"
#include "../include/util.h"

Font::Font() {
}

Font::Font(std::string filename, char characterToGet) : filename(filename), characterToGet(characterToGet) {
    readFont(filename);

}

void Font::readFont(std::string filename) {
    std::ifstream file = std::ifstream(filename, std::ios::in | std::ios::binary);
    std::filesystem::path path = std::filesystem::path(filename); // this is valid.
    fileLength = std::filesystem::file_size(path);
    data = new std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Parse the header! This is used to get the table offsets for the rest of the parsing.
    header = Header(data);

    head = HEADTable(data, header);
    cmap = CMAPTable(data, header, characterToGet);

    // So now, we've got the glyph index we want, and need to use the loca table to convert that into a glyph offset and length!
    auto glyphOffsetAndLength = getGlyphOffset(data, header, head, cmap.glyphIndex);
    glyphOffset = glyphOffsetAndLength.first;
    glyphLength = glyphOffsetAndLength.second;
}

// Mixing levels of abstraction = bad, I know, but parsing the loca table is too trivial to get it's own class imo
std::pair<uint32_t, uint32_t> getGlyphOffset(std::vector<uint8_t> *data, Header header, HEADTable head, uint16_t glyphIndex) {
    // Find the offset of the loca table
    uint32_t offset = header.tables["loca"].tableOffset;

    // Find the number of glyphs in the font
    uint16_t numGlyphs = parseu16(data, header.tables["maxp"].tableOffset+4);

    uint16_t glyphPointerMultiplier = head.indexToLocFormat ? 2 : 1; // WHYYYYYY
    uint16_t glyphPointerSize = head.indexToLocFormat ? 2 : 4;

    uint16_t locaOffset = offset + glyphIndex*glyphPointerSize;

    uint32_t glyphOffset;
    uint32_t nextGlyphOffset;

    if (glyphPointerSize == 2) {
        glyphOffset = parseu16(data, locaOffset);
        nextGlyphOffset = parseu16(data, locaOffset+2);

    } else {
        glyphOffset = parseu32(data, locaOffset);
        nextGlyphOffset = parseu32(data, locaOffset+4);
    }

    glyphOffset *= glyphPointerMultiplier;
    glyphOffset += header.tables["glyf"].tableOffset;

    uint32_t length = nextGlyphOffset - glyphOffset;

    // huh okay that was less trivial than I expected
    return std::pair(glyphOffset, length);
}

Font::~Font() {

}
