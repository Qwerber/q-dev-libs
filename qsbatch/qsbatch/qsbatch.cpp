// qsbatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SDL.h>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

int main(int argc, char* argv[])
{
	SDL_Window* window = 0;
	SDL_Surface* screen = 0;
	window = SDL_CreateWindow("ASD",100,100,SCREENWIDTH,SCREENHEIGHT,SDL_WINDOW_SHOWN);
	screen = SDL_GetWindowSurface(window);
	SDL_Delay(1000);

	SDL_Event e;
	bool running = true;
	while(running)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				running = false;	
			}
		}

		SDL_Rect r;
		r.x= r.y = 0;
		r.w = SCREENWIDTH;
		r.h = SCREENHEIGHT;

		SDL_FillRect(screen, &r, 0xffff0000);
		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

