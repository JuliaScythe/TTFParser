#include <iostream>
#include "../include/Font.h"

// THE BIG ALGORITHM
int main() {

    char characterToDraw = 'a';

    std::string filename = "Roboto.TTF";
    Font font = Font(filename, characterToDraw);
}
