#include <iostream>
#include "../include/Font.h"

int main() {

    char characterToDraw = 'h';

    std::string filename = "Roboto.TTF";
    Font font = Font(filename, characterToDraw);
}
