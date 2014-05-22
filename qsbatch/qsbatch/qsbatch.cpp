// qsbatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include "glbatch.h"
#include "Emitter.h"
#include "Assets.h"
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

void initGL()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	glClearColor(1.0f,1.0f,1.0f,1.0f);
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
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window;
	SDL_GLContext context;
	
	window = SDL_CreateWindow("title",110,110,SCREENWIDTH,SCREENHEIGHT,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	qsb::initGLBatch(SCREENWIDTH, SCREENHEIGHT, 0x000000ff,window);
	
	qass::loadTexture("Forest tileset.png");

	SDL_Event e;

	qsb::Batch b = *qsb::createBatch(5, 6);
	qsb::batch_setProgram(&b,
		qsb::createProgram(
			qsb::createShader(
			GL_VERTEX_SHADER, 
			"#version 130\n"
			"in vec2 LVertexPos2D;"
			"in vec3 c;"
			"out vec3 gs;"
			"uniform mat4 gl_ModelViewMatrix;"
			"void main() { "
			"  gs = c;"
			"  gl_Position = gl_ModelViewMatrix * vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); "
			"}"
			),
			qsb::createShader(
			GL_FRAGMENT_SHADER, 
			"#version 130\nin vec3 gs; out vec4 LFragment; void main() { LFragment  = vec4( gs.x, gs.y, gs.z, 1 ); }"
			)));

	qsb::batch_generateAttributeData(&b, "LVertexPos2D{ff}c{fff}");

	qsb::batch_PrintAttributeData(&b);

	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 1);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushIndex(&b, 0);
	
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 111);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 1);
	qsb::batch_pushIndex(&b, 1);
	
	qsb::batch_pushVertData(&b, 111);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 1);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushIndex(&b, 2);
	
	qsb::batch_pushVertData(&b, 111);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 1);
	qsb::batch_pushIndex(&b, 3);
	
	qsb::batch_pushVertData(&b, 111);
	qsb::batch_pushVertData(&b, 111);
	qsb::batch_pushVertData(&b, 1);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushIndex(&b, 4);
	
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 111);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushVertData(&b, 1);
	qsb::batch_pushVertData(&b, 0);
	qsb::batch_pushIndex(&b, 5);

	//qsb::drawBatch(&b);

	kc8::Emitter2D* em = kc8::createEmitter(400000);
	srand(time(0));
	int i = 400000;
	while (i--)
	{
		int x = (rand() % 100) - 50;
		int y = (rand() % 100) - 50;

		kc8::emitParticles(em, 1, { 200, 200 }, { 0.05 * x, -0.02 * y });
	}
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
		
		kc8::updateEmitter(em);
		
		kc8::renderEmitter(em);
		

		SDL_GL_SwapWindow(window);

		

	}
	
	return 0;
}

