#ifndef __SDL_HPP__
#define __SDL_HPP__

#include "cowichan.hpp"

#include <cstdlib>
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>

typedef unsigned int uint32;

class Graphics {
private:

	static SDL_Surface *screen;
	static int _w;

public:

	static bool init(int width, int height) {

		_w = width;

		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			return false;
		}

		screen = SDL_SetVideoMode(width * 2, height * 2, 32, SDL_DOUBLEBUF | SDL_SWSURFACE);
		if (screen == NULL) {
			return false;
		}
		
		return true;
	}

	static void draw(BoolMatrix bm) {
		uint32* index = (uint32*) screen->pixels;
		int v;
		for (int y = 0; y < Cowichan::NELTS * 2; ++y) {
			for (int x = 0; x < Cowichan::NELTS * 2; ++x) {
		
				index++;
				v = (MATRIX(bm, y/2, x/2) ? 255 : 0);
//				std::cout << v << std::endl;
				*index = SDL_MapRGB(screen->format, v, v, v);
				
			}
		}
		SDL_Flip(screen);
	}
/*	
	static void setPixel(int x, int y, int r, int g, int b) {
		uint32* index = (uint32*) screen->pixels;
		index += (_w * y) + x;
	}
*/	
	static void delay(int delay) {
		SDL_Delay(delay);
	}
	
	static void deinit() {
	
		SDL_Quit();
	
	}	
	
};

SDL_Surface *Graphics::screen = NULL;
int Graphics::_w = 0;

#endif

