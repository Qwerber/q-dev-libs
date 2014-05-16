#pragma once
	
#include <SDL.h>
#include <SDL_opengl.h>

namespace qsb
{
	extern int OK;
	extern int FAIL;

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
	void batch_generateAttributeData(Batch* _batch, char* _data);
	void batch_PrintAttributeData(Batch* _batch);

#pragma region "push functions"

	inline void batch_pushVertex(Batch* _batch, GLfloat _x, GLuint _i1)
	{
		_batch->vertextData[_batch->vbDataPointer++] = _x;
		_batch->indexData[_batch->ibDataPointer++] = _i1;
		_batch->vertexCount++;
	}

	inline void batch_pushVertex(Batch* _batch, GLfloat _x, GLfloat _y, GLuint _i1)
	{
		_batch->vertextData[_batch->vbDataPointer++] = _x;
		_batch->vertextData[_batch->vbDataPointer++] = _y;
		_batch->indexData[_batch->ibDataPointer++] = _i1;
		_batch->vertexCount++;
	}

	inline void batch_pushVertex(Batch* _batch, GLfloat _x, GLfloat _y, GLfloat _z, GLuint _i1)
	{
		_batch->vertextData[_batch->vbDataPointer++] = _x;
		_batch->vertextData[_batch->vbDataPointer++] = _y;
		_batch->vertextData[_batch->vbDataPointer++] = _z;
		_batch->indexData[_batch->ibDataPointer++] = _i1;
		_batch->vertexCount++;
	}

	inline void batch_pushVertex(Batch* _batch, GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w, GLuint _i1)
	{
		_batch->vertextData[_batch->vbDataPointer++] = _x;
		_batch->vertextData[_batch->vbDataPointer++] = _y;
		_batch->vertextData[_batch->vbDataPointer++] = _z;
		_batch->vertextData[_batch->vbDataPointer++] = _w;
		_batch->indexData[_batch->ibDataPointer++] = _i1;
		_batch->vertexCount++;
	}

	inline void batch_pushVertex(Batch* _batch, GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w, GLfloat _u, GLuint _i1)
	{
		_batch->vertextData[_batch->vbDataPointer++] = _x;
		_batch->vertextData[_batch->vbDataPointer++] = _y;
		_batch->vertextData[_batch->vbDataPointer++] = _z;
		_batch->vertextData[_batch->vbDataPointer++] = _w;
		_batch->vertextData[_batch->vbDataPointer++] = _u;
		_batch->indexData[_batch->ibDataPointer++] = _i1;
		_batch->vertexCount++;
	}

	inline void batch_pushVertex(Batch* _batch, GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w, GLfloat _u, GLfloat _v, GLuint _i1)
	{
		_batch->vertextData[_batch->vbDataPointer++] = _x;
		_batch->vertextData[_batch->vbDataPointer++] = _y;
		_batch->vertextData[_batch->vbDataPointer++] = _z;
		_batch->vertextData[_batch->vbDataPointer++] = _w;
		_batch->vertextData[_batch->vbDataPointer++] = _u;
		_batch->vertextData[_batch->vbDataPointer++] = _v;
		_batch->indexData[_batch->ibDataPointer++] = _i1;
		_batch->vertexCount++;
	}

	inline void batch_pushVertex(Batch* _batch, GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w, GLfloat _u, GLfloat _v, GLfloat _t, GLuint _i1)
	{
		_batch->vertextData[_batch->vbDataPointer++] = _x;
		_batch->vertextData[_batch->vbDataPointer++] = _y;
		_batch->vertextData[_batch->vbDataPointer++] = _z;
		_batch->vertextData[_batch->vbDataPointer++] = _w;
		_batch->vertextData[_batch->vbDataPointer++] = _u;
		_batch->vertextData[_batch->vbDataPointer++] = _v;
		_batch->vertextData[_batch->vbDataPointer++] = _t;
		_batch->indexData[_batch->ibDataPointer++] = _i1;
		_batch->vertexCount++;
	}
#pragma endregion

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