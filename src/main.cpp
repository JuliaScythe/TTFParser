#include <iostream>
#include "../include/Font.h"

#include <SDL2/SDL.h>
#include "../SDL_gfx/SDL2_gfxPrimitives.h"
#include <algorithm>

const int WIDTH = 400;
const int HEIGHT = 400;

const double SCALE = 0.1;

void drawFont(Font font, SDL_Renderer* renderer);

int main() {

    SDL_Window* window = nullptr;		// Create pointers for our window and renderer -- these will remain as null
    SDL_Renderer* renderer = nullptr;   // if window or renderer creation fails.

    SDL_Init(SDL_INIT_VIDEO);			// Attempt to initialize SDL

    window = SDL_CreateWindow( "Font Renderer",		// Attempt to create the window...
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WIDTH, HEIGHT,
                               SDL_WINDOW_SHOWN );

    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);  // ...and the renderer

    // Render a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    char characterToDraw = 'A';

    std::string filename = "Roboto.TTF";
    Font font = Font(filename, characterToDraw);	// Create the font object (this will also parse the file)


    bool quit = false;
    // Do things here
    while(!quit) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);		// Clear the screen

        drawFont(font, renderer);  // Render our font to the buffer
		SDL_RenderPresent(renderer);	// Show the buffer on the screen

		// BENCHMARKING
		quit=true;

		SDL_StartTextInput();
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if(ev.type == SDL_TEXTINPUT) {
				characterToDraw = ev.text.text[0];
				font = Font(filename, characterToDraw);
				SDL_StopTextInput();
				SDL_StartTextInput(); // Restart text input to avoid any weird text editing stuff
			}

			if(ev.type == SDL_QUIT) {
				quit = true;
			}
		}
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void drawFont(Font font, SDL_Renderer *renderer) {
	// Pick a sensible place to start
	double locX = 1400;
	double locY = -2500;

	std::vector<Point> points = {};
	std::vector<int> endpoints = {};

	for(int i=0; i<font.glyph.points.size(); i++) {

		// If this point would be the last point of a contour, we need to not draw a line here.
		// We add it the current index to the endpoints array so we know when not to draw a line.
		if (std::find(font.glyph.endPointsOfContours.begin(), font.glyph.endPointsOfContours.end(), i) != font.glyph.endPointsOfContours.end()) {
			endpoints.push_back(points.size());
		}

		Point* thisPoint = &font.glyph.points.at(i);
		Point* nextPoint = nullptr;
		try {
			nextPoint = &font.glyph.points.at(i+1);
		} catch (std::out_of_range &e) {
			nextPoint = &font.glyph.points.at(0); // If we're at the last glyph, go back to the start
		}

		// Insert the point
		locX += thisPoint->deltaX;
		locY += thisPoint->deltaY;
		points.emplace_back(locX, -locY, thisPoint->flag);



		if(!thisPoint->flag.onCurvePoint && !nextPoint->flag.onCurvePoint) {
			// Insert a phantom point between the two
			auto deltaX = nextPoint->deltaX / 2;
			auto deltaY = nextPoint->deltaY / 2;

			points.emplace_back(locX+deltaX, -(locY+deltaY), PointFlag(0b00000001));
		}

	}

	// Sort the points into contours
	std::vector<std::vector<Point>> contours;

	int i = 0;
	for(auto maxI : endpoints) {
		std::cout << "Starting contour, will end at point #" << maxI << std::endl;
		std::vector<Point> currentContour = {};
		int s = i;
		for(; i <= maxI; i++) {
			std::cout << "Adding point #" << i << std::endl;
			currentContour.emplace_back(points[i]);
		}
		std::cout << "Readding point #" << s << std::endl;
		currentContour.emplace_back(points[s]);
		currentContour.emplace_back(points[s]);
		contours.push_back(currentContour);
	}

	std::cout << "Done adding points" << std::endl;

	// Draw each contour to the screen
	for(auto contour : contours) {
		std::cout << "Starting new contour" << std::endl;
		for(int j=0; j < contour.size()-1; j++) {
			Point* currentPoint = &contour[j];
			Point* controlPoint = nullptr;
			Point* nextPoint;
			if(!contour[j+1].flag.onCurvePoint) {
				controlPoint = &contour[j+1];
				nextPoint = &contour[j+2];
				j++;
			} else {
				nextPoint = &contour[j+1];
			}

			if(controlPoint) {
				std::cout << "Drawing curve from (" << currentPoint->deltaX << ", " << currentPoint->deltaY << ") to (" << nextPoint->deltaX << ", " << nextPoint->deltaY << ").\n";
				double pX[] = {
						static_cast<double>(currentPoint->deltaX) * SCALE,
						static_cast<double>(controlPoint->deltaX) * SCALE,
						static_cast<double>(nextPoint->deltaX) * SCALE
				};

				double pY[] = {
						static_cast<double>(currentPoint->deltaY) * SCALE,
						static_cast<double>(controlPoint->deltaY) * SCALE,
						static_cast<double>(nextPoint->deltaY) * SCALE
				};
				aaBezierRGBA(renderer, pX, pY, 3, 20, 1.0, 0, 0, 0, 255);
			} else {
				std::cout << "Drawing line from (" << currentPoint->deltaX << ", " << currentPoint->deltaY << ") to (" << nextPoint->deltaX << ", " << nextPoint->deltaY << ").\n";
				aalineRGBA(renderer, currentPoint->deltaX * SCALE, currentPoint->deltaY * SCALE, nextPoint->deltaX * SCALE, nextPoint->deltaY * SCALE, 0, 0, 0, 255);
			}
		}
	}
}
