#include <fstream>
#include <filesystem>
#include <iostream>
#include "../include/Font.h"
#include "../include/util.h"
#include "../include/log.h"

Font::Font() {
}

Font::Font(std::string filename, char characterToGet) : filename(filename), characterToGet(characterToGet) {
    info("Opening font: ", filename);
    info("-> Looking for character: ", std::string(1, characterToGet));

    readFont(filename);
}

void Font::readFont(std::string filename) {

    std::ifstream file = std::ifstream(filename, std::ios::in | std::ios::binary);
    std::filesystem::path path = std::filesystem::path(filename);
    fileLength = std::filesystem::file_size(path);
    data = new std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    info("-> Read " + std::to_string(data->size()) + " bytes.", "");

    // Parse the header! This is used to get the table offsets for the rest of the parsing.
    header = Header(data);

    head = HEADTable(data, header);
    cmap = CMAPTable(data, header, characterToGet);

    // So now, we've got the glyph index we want, and need to use the loca table to convert that into a glyph offset and length!
    auto glyphOffsetAndLength = getGlyphOffset(data, header, head, cmap.glyphIndex);
    glyphOffset = glyphOffsetAndLength.first;
    glyphLength = glyphOffsetAndLength.second;

    // And now we need to actually parse the glyph!
    glyph = Glyph(data, glyphOffset, glyphLength);


}

// Mixing levels of abstraction = bad, I know, but parsing the loca table is too trivial to get it's own class imo
std::pair<uint32_t, uint32_t> getGlyphOffset(std::vector<uint8_t> *data, Header header, HEADTable head, uint16_t glyphIndex) {
    // Find the offset of the loca table
    uint32_t offset = header.tables["loca"].tableOffset;

    info("Glyph Index: ", std::to_string(glyphIndex));

    uint16_t glyphPointerMultiplier = head.indexToLocFormat ? 1 : 2; // why is the spec life this :(
    uint16_t glyphPointerSize = head.indexToLocFormat ? 4 : 2;

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
    nextGlyphOffset *= glyphPointerMultiplier;

    uint32_t length = nextGlyphOffset - glyphOffset;

    glyphOffset += header.tables["glyf"].tableOffset;

    // huh okay that was less trivial than I expected
    return std::pair(glyphOffset, length);
}

Font::~Font() {

}
