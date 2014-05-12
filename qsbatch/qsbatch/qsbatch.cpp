// qsbatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include "glbatch.h"


const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;


void initGL()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	glClearColor(1.0f,0.0f,1.0f,1.0f);
}

void initViewport(int _width, int _height)
{
	glViewport(0,0,(GLsizei)_width,(GLsizei)_height);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0,_width,_height,0,0,1);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	/*SDL_Window* window = 0;
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
	SDL_Quit();*/ 

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window;
	SDL_GLContext context;
	
	window = SDL_CreateWindow("title",110,110,SCREENWIDTH,SCREENHEIGHT,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	qsb::initGLBatch(SCREENWIDTH, SCREENHEIGHT, 0x660000ff,window);

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

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_QUADS);
			glColor3f(1.0f,1.0f,0);
			glVertex2i(0,0);
			glVertex2i(110,0);
			glVertex2i(110,110);
			glVertex2i(0,110);
		glEnd();

		SDL_GL_SwapWindow(window);

	}

	return 0;
}

