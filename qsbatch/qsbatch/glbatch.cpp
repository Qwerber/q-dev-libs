#include "stdafx.h"

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

namespace qsb
{

	static SDL_Window* window;
	static SDL_GLContext context;
	static int screenWidth;
	static int screenHeight;

	struct Batch
	{
		GLuint vertexShader;
		GLuint fragmentShader;
	};

	int initGLBatch(int _screenWidth, int _screenHeight, int _defaultColor, SDL_Window* _window)
	{
		screenWidth = screenWidth;
		screenHeight = _screenHeight;
		window = _window;

		context = SDL_GL_CreateContext(_window);

		if(!context)
		{
			printf("Couldn't create context: %s\n", SDL_GetError());
			return 0;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		int r = _defaultColor >> 24 & 0xff;
		int g = _defaultColor >> 16 & 0xff;
		int b = _defaultColor >> 8 & 0xff;
		int a = _defaultColor & 0xff;

		glClearColor((GLclampf)r/255,(GLclampf)g/255,(GLclampf)b/255,(GLclampf)a/255);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glOrtho(0,_screenWidth, _screenHeight, 0, -10,10);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glewExperimental = GL_TRUE;
		GLenum error = glewInit();
		if(error!=GLEW_OK)
		{
			printf("Error initializing glew: %s\n", glewGetErrorString(error));
			return 0;
		}

		return 1;
	}

	int getScreenWidth()
	{
		return screenWidth;
	}

	int getScreenHeight()
	{
		return screenHeight;
	}

}