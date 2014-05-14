#include "stdafx.h"

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include "glbatch.h"

namespace qsb
{

	static SDL_Window* window;
	static SDL_GLContext context;

	static int screenWidth;
	static int screenHeight;
	
	static std::vector<Batch> batchList;
	static int numBatches;

	static GLuint vertexBuffer;
	static GLuint indexBuffer;

#pragma region "internal"
	void printShaderLog( GLuint shader )
	{
		//Make sure name is shader
		if( glIsShader( shader ) )
		{
			//Shader log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;
		
			//Get info string length
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
		
			//Allocate string
			char* infoLog = new char[ maxLength ];
		
			//Get info log
			glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
			if( infoLogLength > 0 )
			{
				//Print Log
				printf( "%s\n", infoLog );
			}

			//Deallocate string
			delete[] infoLog;
		}
		else
		{
			printf( "Name %d is not a shader\n", shader );
		}
	}

	void printProgramLog( GLuint program )
	{
		//Make sure name is shader
		if( glIsProgram( program ) )
		{
			//Program log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;
		
			//Get info string length
			glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		
			//Allocate string
			char* infoLog = new char[ maxLength ];
		
			//Get info log
			glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
			if( infoLogLength > 0 )
			{
				//Print Log
				printf( "%s\n", infoLog );
			}
		
			//Deallocate string
			delete[] infoLog;
		}
		else
		{
			printf( "Name %d is not a program\n", program );
		}
	}
#pragma endregion


#pragma region "batch funcs"

	VertexAttribute createVA()
	{
		VertexAttribute ret;
		ret.location = -1;
		ret.dim = 1;
		ret.skip = 0;
		ret.start = 0;
		ret.type = GL_FLOAT;

		return ret;
	}

	Batch createBatch(int _dataPerVertex, int _length)
	{
		
		Batch batch = {};
		
		batch.dataPerVertex = _dataPerVertex;
		batch.length = _length;
		
		batch.shaderProgram = 0;
		
		batch.vertextData = (GLfloat*) malloc(_length * _dataPerVertex * sizeof(GLfloat));
		batch.indexData = (GLuint*) malloc(_length * sizeof(GLuint));

		batch.vbDataPointer = 0;
		batch.ibDataPointer = 0;

		batch.numAttributes = 0;

		return batch;
	
	}

	// this does not destroy the shaderProgram, it needs to be done manually
	void destroyBatch(Batch* _batch)
	{

		free(_batch->vertextData);
		free(_batch->indexData);

	}

	void batch_setProgram(Batch* _batch, GLuint _program)
	{
		
		_batch->shaderProgram = _program;
	
	}

	int pushBatch(Batch _batch)
	{
		batchList.push_back(_batch);
		numBatches ++;
		return batchList.size() - 1;
	}

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
#pragma endregion

	int initGLBatch(int _screenWidth, int _screenHeight, int _defaultColor, SDL_Window* _window)
	{
		////////////////////
		//   Batch things

		// store values
		screenWidth = screenWidth;
		screenHeight = _screenHeight;
		window = _window;

		batchList = std::vector<Batch>();
		numBatches = 0;

		////////////////////
		//   OGL things

		// create ogl context
		context = SDL_GL_CreateContext(_window);
		
		if(!context)
		{
			printf("Couldn't create context: %s\n", SDL_GetError());
			return 0;
		}

		// set ogl version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		//grab color values from hex int
		int r = _defaultColor >> 24 & 0xff;
		int g = _defaultColor >> 16 & 0xff;
		int b = _defaultColor >> 8 & 0xff;
		int a = _defaultColor & 0xff;

		glClearColor((GLclampf)r/255,(GLclampf)g/255,(GLclampf)b/255,(GLclampf)a/255);

		//set the right matrix values
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glOrtho(0,_screenWidth, _screenHeight, 0, -10,10);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		// init glew
		glewExperimental = GL_TRUE;
		GLenum error = glewInit();
		if(error!=GLEW_OK)
		{
			printf("Error initializing glew: %s\n", glewGetErrorString(error));
			return 0;
		}

		//create ibo and vbo
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);

		return 1;
	
	}

	int drawBatch(Batch _b)
	{
		glUseProgram(_b.shaderProgram);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, _b.vbDataPointer * sizeof(GLfloat), _b.vertextData, GL_STATIC_DRAW);
		
		int i = _b.numAttributes;
		while(i--)
		{
			glEnableVertexAttribArray(_b.attributes[i].location);
			glVertexAttribPointer(_b.attributes[i].location, _b.attributes[i].dim, _b.attributes[i].type, GL_FALSE, 
				_b.dataPerVertex * sizeof(GLfloat), 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);

		//unbind etc

		return 0;
	}

	int drawAllBatches()
	{
		int len = batchList.size();
		int i = 0;
		while(i < len)
		{
			Batch b = batchList[i];

			drawBatch(b);

			i++;
		}

		return 0;
	}

	GLuint createShader(GLuint _shaderType, const GLchar* shaderCode)
	{
		
		// create and compile shader
		GLuint shader = glCreateShader(_shaderType);
		const GLchar* source[] = { shaderCode };
		glShaderSource(shader,(GLsizei)1, source, 0);
		glCompileShader(shader);

		// check compile status
		GLint status = GL_FALSE;
		glGetShaderiv(shader,GL_COMPILE_STATUS, &status);
		if(!status)
		{
			printf("Unable to compile shader\n");
			printShaderLog(shader);
		}
		
		return shader;
	
	}

	GLuint createProgram(GLuint _vertexShader, GLuint _fragmentShader)
	{
		
		GLuint program = glCreateProgram();
		glAttachShader(program, _vertexShader);
		glAttachShader(program, _fragmentShader);
		glLinkProgram(program);
		GLint status = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if(!status)
		{
			printf("Unable to link program\n");
			printProgramLog(program);
		}

		return program;
	
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