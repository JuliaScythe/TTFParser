//
// Created by jake on 12/10/2020.
//
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../include/Font.h"

Font::Font() {
}

Font::Font(std::string filename) : filename(filename) {
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
    cmap = CMAPTable(data, header);
}

Font::~Font() {

}
