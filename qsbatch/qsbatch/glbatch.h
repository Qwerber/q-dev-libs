#pragma once
	
#include <SDL.h>
#include <SDL_opengl.h>

namespace qsb
{
	struct VertexAttribute
	{
		GLint location;
		GLint dim;
		GLint start;
		GLint skip;
		GLenum type;
	};

	struct Batch
	{
	
		GLuint dataPerVertex;
		GLuint length;

		GLuint shaderProgram;
		GLint vertexLocation;

		GLfloat* vertextData;
		GLuint* indexData;

		GLuint vbDataPointer;
		GLuint ibDataPointer;
		GLuint vertexCount;

		VertexAttribute attributes[16];
		int numAttributes;
		
	};

	Batch createBatch(int _dataPerVertex, int _length);
	void destroyBatch(Batch* _batch);
	void batch_setProgram(Batch* _batch, GLuint _program);

	int initGLBatch(int _screenWidth, int _screenHeight, int _defaultColor, SDL_Window* _window);

	GLuint createShader(GLuint _shaderType, const GLchar* _shaderCode);
	GLuint createProgram(GLuint _vertexShader, GLuint _fragmentShader);

	// Once you set the screen dimensions once through initialization you cannot change them again
	int getScreenWidth();
	int getScreenHeight();

	int pushBatch(Batch _batch);
	int drawBatch(Batch _b);
	int drawAllBatches();
	int display();

	int cleanup();

	// debug functions


}