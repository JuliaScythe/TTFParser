//
// Created by jake on 12/10/2020.
//

#ifndef NEA_FONT_H
#define NEA_FONT_H
#include <string>
#include <vector>
#include "Header.h"
#include "HEADTable.h"
#include "CMAPTable.h"

class Font {
public:
    Font();
    Font(std::string filename);
    ~Font();

    void parse();
private:

    void readFont(std::string filename);

    std::string filename;

    int fileLength;
    std::vector<uint8_t>* data;

    Header header;
    HEADTable head;
    CMAPTable cmap;


};


#endif //NEA_FONT_H
