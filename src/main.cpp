#include <iostream>
#include "../include/Font.h"

#include <SDL2/SDL.h>
#include "../SDL_gfx/SDL2_gfxPrimitives.h"
#include <algorithm>

const int WIDTH = 1000;
const int HEIGHT = 1000;

const double SCALE = 0.1;

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
    char characterToDraw = '4';

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
    double locX = 5000;
    double locY = -5000;

    double lastX = locX;
    double lastY = locY;

    std::vector<Point> points = {};
    std::vector<int> endpoints = {};

    for(int i=0; i<font.glyph.points.size(); i++) {

    	Point* thisPoint = &font.glyph.points.at(i);
    	Point* nextPoint = nullptr;
    	try {
    		nextPoint = &font.glyph.points.at(i+1);
    	} catch (std::out_of_range &e) {
    	}

    	// Insert the point
    	locX += thisPoint->deltaX;
    	locY += thisPoint->deltaY;
    	points.emplace_back(locX, -locY, thisPoint->flag);

		// If this point would be the last point of a contour, we need to not draw a line here.
		// We add it the current index to the endpoints array so we know when not to draw a line.
		if (std::find(font.glyph.endPointsOfContours.begin(), font.glyph.endPointsOfContours.end(), i) != font.glyph.endPointsOfContours.end()) {
			endpoints.push_back(points.size());
		}

    	if(!thisPoint->flag.onCurvePoint && !nextPoint->flag.onCurvePoint) {
    		// Insert a phantom point between the two
    		auto deltaX = nextPoint->deltaX / 2;
			auto deltaY = nextPoint->deltaY / 2;

			points.emplace_back(locX+deltaX, -(locY+deltaY), PointFlag(0b00000001));
    	}

	}

    // Add a final point, to ensure the curve is closed
	points.emplace_back(500, 500, PointFlag(0b00000001));

    aacircleRGBA(renderer, 500, 100, 2, 0, 0, 0, 255);

    int nextEndpoint = endpoints[0];
    int endpointIndex = 0;

    Point* startPoint = &points[0];

	for(int i=0; i<points.size()-2; i++) {

		Point* otherEnd = &points.at(i+1);

		if(i == nextEndpoint) {
			std::cout << "END" << std::endl;
			otherEnd = startPoint;
			startPoint = &points.at(i+1);
			endpointIndex++;
			if (endpointIndex >= endpoints.size()) {
				nextEndpoint = -1;
			} else {
				nextEndpoint = endpoints[endpointIndex];
			}
		}


		if(points.at(i+1).flag.onCurvePoint) {
			std::cout << points.at(i).deltaY * SCALE << std::endl;
			aalineRGBA(renderer, points.at(i).deltaX * SCALE, points.at(i).deltaY * SCALE, points.at(i+1).deltaX * SCALE, points.at(i+1).deltaY * SCALE, 0, 0, 0, 255);
		} else {
			double pX[] = {
					static_cast<double>(points.at(i).deltaX) * SCALE,
					static_cast<double>(points.at(i+1).deltaX) * SCALE,
					static_cast<double>(points.at(i+2).deltaX) * SCALE
			};

			double pY[] = {
					static_cast<double>(points.at(i).deltaY) * SCALE,
					static_cast<double>(points.at(i+1).deltaY) * SCALE,
					static_cast<double>(points.at(i+2).deltaY) * SCALE
			};
			aaBezierRGBA(renderer, pX, pY, 3, 20, 1.0, 0, 0, 0, 255);

		}
	}

/*
		// For each point of the font...
    for(auto point : font.glyph.points) {
        if(point.flag.onCurvePoint) {
			aacircleRGBA(renderer, locX, locY, 5, 0, 0, 0, 255);
			aalineRGBA(renderer, locX, locY, lastX, lastY, 0, 0, 0, 255);
			lastX = locX;
			lastY = locY;
		}
        locX += point.deltaX / 10;
        locY -= point.deltaY / 10; // Y coords are inverted for some reason
    }

    double pX[] = {100, 10, 10};
    double pY[] = {10, 10, 100};

    aaBezierRGBA(renderer, pX, pY, 3, 20, 5.0, 0, 0, 0, 255);
    */
}