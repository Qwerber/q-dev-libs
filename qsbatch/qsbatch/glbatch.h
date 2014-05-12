#pragma once
	
#include <SDL.h>
#include <SDL_opengl.h>

namespace qsb
{
struct Batch;
Batch createBatch();
void batch_setProgram(GLuint _program);

int initGLBatch(int _screenWidth, int _screenHeight, int _defaultColor, SDL_Window* _window);

GLuint createShader(GLuint _shaderType, GLchar* _shaderCode);
GLuint createProgram(GLuint _vertexShader, GLuint _fragmentShader);

// Once you set the screen dimensions once through initialization you cannot change them again
int getScreenWidth();
int getScreenHeight();

int submitBatch(Batch _batch);
int drawAllBatches();
int display();

int cleanup();


}