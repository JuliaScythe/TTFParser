#include <iostream>
#include "../include/Font.h"

#include <SDL2/SDL.h>
#include "../SDL_gfx/SDL2_gfxPrimitives.h"


const int WIDTH = 600;
const int HEIGHT = 600;

void drawFont(Font font, SDL_Renderer* renderer);

int main() {
/** test code
    char characterToDraw = 'a';

    std::string filename = "Roboto.TTF";
    Font font = Font(filename, characterToDraw);
    */

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow( "Font Renderer",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WIDTH, HEIGHT,
                               SDL_WINDOW_SHOWN );

    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    // Render a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    lineRGBA(renderer, 0, 0, WIDTH, HEIGHT, 0x00, 0x00, 0x00, 0xFF);
    for (int i = 0; i < HEIGHT; i+=2) {
        lineRGBA(renderer, 0, i, WIDTH, HEIGHT, 0x00+i, 0xFF-i, 0x00+i, 0xFF);
    }

    SDL_RenderPresent(renderer);

    // Render the font
    char characterToDraw = 'I';

    std::string filename = "Roboto.TTF";
    Font font = Font(filename, characterToDraw);


    bool quit = false;
    // Do things here
    while(!quit) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawFont(font, renderer);
        SDL_RenderPresent(renderer);
        break;
    }
    SDL_Delay(10000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawFont(Font font, SDL_Renderer* renderer) {

    // Pick a sensible place to start
    double locX = 300;
    double locY = 300;

    // For each point of the font...
    for(auto point : font.glyph.points) {
        if(point.flag.onCurvePoint)
            aacircleRGBA(renderer, locX, locY, 5, 0, 0, 0, 255);
        locX += point.deltaX / 100;
        locY += point.deltaY / 100;

    }


    double pX[] = {100, 10, 10};
    double pY[] = {10, 10, 100};

    aaBezierRGBA(renderer, pX, pY, 3, 20, 5.0, 0, 0, 0, 255);
}


