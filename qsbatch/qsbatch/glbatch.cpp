#include "stdafx.h"

#include <string.h>
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <Windows.h>
#include "glbatch.h"
#include "utils.h"

namespace qsb
{
	// Constants for function success/fail
	const int OK = 0;
	const int FAIL = 1;

	// SDL objects
	static SDL_Window* window;
	static SDL_GLContext context;

	// Properties
	static int screenWidth;
	static int screenHeight;
	
	// Batch stuff
	static std::vector<Batch*> batchList;
	static int numBatches;

	// OpenGL objects
	static GLuint vertexBuffer;
	static GLuint indexBuffer;

	//fix this later
	GLuint types[256];

#pragma region "lazyfoo functions"
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


	// create vertexAttribute
	VertexAttribute createVA()
	{

		VertexAttribute ret;
		ret.location = -1;
		ret.dim = 1;
		ret.start = 0;
		ret.type = GL_FLOAT;

		return ret;

	}

	Batch* createBatch(int _dataPerVertex, int _length)
	{
		
		Batch* batch = (Batch*)malloc(sizeof(Batch));
		
		batch->dataPerVertex = _dataPerVertex;
		batch->length = _length;
		
		batch->shaderProgram = 0;
		
		batch->vertextData = (GLfloat*) malloc(_length * _dataPerVertex * sizeof(GLfloat));
		batch->indexData = (GLuint*) malloc(_length * sizeof(GLuint));

		batch->vbDataPointer = batch->vertextData;
		batch->ibDataPointer = batch->indexData;
		batch->indexValue = 0;

		batch->numAttributes = 0;
		return batch;
	
	}

	// this does not destroy the shaderProgram, it needs to be done manually
	void destroyBatch(Batch* _batch)
	{

		free(_batch->vertextData);
		free(_batch->indexData);

	}

	void batch_reset(Batch* _batch)
	{
		_batch->vbDataPointer = _batch->vertextData;
		_batch->ibDataPointer = _batch->indexData;
		_batch->vertexCount = 0;
		_batch->indexValue = 0;
	}

	void batch_setProgram(Batch* _batch, GLuint _program)
	{
		
		_batch->shaderProgram = _program;
	
	}

	void batch_PrintAttributeData(Batch* _batch)
	{
		int i = _batch->numAttributes;
		printf_s("shader: \n");
		while (i--)
		{
			printf_s("location:%d | dimension:%d | type:%d | start:%d\n",
					 _batch->attributes[i].location,
					 _batch->attributes[i].dim,
					 _batch->attributes[i].type,
					 _batch->attributes[i].start);
		}
	}

	// format: {attrib}fff{attrib2}ii.
	//
	// where: f is GLfloat
	//        i is GLint

	// TODO: Switch to glBindAttribLocation if possible
	void batch_generateAttributeData(Batch* _batch, char* _data)
	{
		char* copy = _strdup(_data);

		int ANAME = 0, ATYPES = 1, state = 0, dataSize = 0;
		char* tok, *save;
		
		tok = strtok_s(copy, "{", &save);
		while (tok)
		{
			if (ANAME == state)
			{
				_batch->attributes[_batch->numAttributes].location = glGetAttribLocation(_batch->shaderProgram, tok);;
				tok = strtok_s(0, "}" ,&save);
				state = ATYPES;
			}
			else if (ATYPES == state)
			{
				int size = strlen(tok);
				_batch->attributes[_batch->numAttributes].dim = size;
				_batch->attributes[_batch->numAttributes].type = (tok[0] == 'i') ? GL_INT : GL_FLOAT;
				_batch->attributes[_batch->numAttributes].start = dataSize;
				dataSize += size;
				_batch->numAttributes++;

				tok = strtok_s(0, "{", &save);
				state = ANAME;
			}
			
		}
	}

	int pushBatch(Batch* _batch)
	{
		batchList.push_back(_batch);
		numBatches ++;
		return batchList.size() - 1;
	}
#pragma endregion

#pragma region "GLBatch"

	GLBatch::GLBatch(GLuint _bytesPerVertex, GLuint _vertexLength, GLuint _indexLength):
		bytesPerVertex(_bytesPerVertex),
		vertexLength(_vertexLength),
		indexLength(_indexLength)
	{
		shaderProgram = NULL;
		vertexData = new GLubyte[vertexLength * _bytesPerVertex];
		indexData = new GLuint[indexLength];

		vbDataPointer = vertexData;
		ibDataPointer = indexData;
		writingIndex = 0;

		numAttributes = 0;
	}

	void GLBatch::reset()
	{
		ibDataPointer = indexData;
		vbDataPointer = vertexData;
		writingIndex = 0;
	}

	void GLBatch::setProgram(GLuint _program)
	{
		shaderProgram = _program;
	}

	void GLBatch::generateAttributeData(char* _data)
	{
		char* copy = _strdup(_data);

		int ANAME = 0, ATYPES = 1, state = 0, dataSize = 0;
		char* tok, *save;

		tok = strtok_s(copy, "{", &save);
		while (tok)
		{
			if (ANAME == state)
			{
				attributes[numAttributes].location = glGetAttribLocation(shaderProgram, tok);;
				tok = strtok_s(0, "}", &save);
				state = ATYPES;
			}
			else if (ATYPES == state)
			{
				int size = strlen(tok) - 1;
				attributes[numAttributes].dim = size;
				attributes[numAttributes].type = types[tok[1]];
				attributes[numAttributes].start = dataSize;
				dataSize += types[tok[0]] * size;
				numAttributes++;

				tok = strtok_s(0, "{", &save);
				state = ANAME;
			}

		}
	}

	GLBatch::~GLBatch()
	{
		if (indexData)
		{
			delete[] indexData;
			indexData = NULL;
		}
		if (vertexData)
		{
			delete[] vertexData;
			vertexData = NULL;
		}
	}

#pragma endregion

	int initGLBatch(int _screenWidth, int _screenHeight, int _defaultColor, SDL_Window* _window)
	{
		////////////////////
		//   Batch things

		types['f'] = GL_FLOAT;
		types['b'] = GL_UNSIGNED_BYTE;
		types['s'] = GL_UNSIGNED_SHORT;
		types['S'] = GL_SHORT;

		types['1'] = 1;
		types['1'] = 2;
		types['1'] = 3;
		types['1'] = 4;

		// store values
		screenWidth = screenWidth;
		screenHeight = _screenHeight;
		window = _window;

		batchList = std::vector<Batch*>();
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

		glEnable(GL_TEXTURE_2D);

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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//create ibo and vbo
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);

		return 1;
	
	}

	// draw a single batch 
	int drawBatch(Batch* _b)
	{
		unsigned int t = GetTickCount();
		glUseProgram(_b->shaderProgram);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, (GLuint)(_b->vbDataPointer - _b->vertextData) * sizeof(GLfloat), _b->vertextData, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLuint)(_b->ibDataPointer - _b->indexData) * sizeof(GLuint), _b->indexData, GL_DYNAMIC_DRAW);
		//printf("bufferdata: %u\n", GetTickCount() - t);

		t = GetTickCount();
		{
			int i = _b->numAttributes;
			while(i--)
			{
				glEnableVertexAttribArray(_b->attributes[i].location);
				glVertexAttribPointer(_b->attributes[i].location, 
									  _b->attributes[i].dim,
									  _b->attributes[i].type, 
									  GL_FALSE,
									  _b->dataPerVertex * sizeof(GLfloat), 
									  (void*)(_b->attributes[i].start * sizeof(GLfloat)));
			}
		}
		//printf("attrib: %u\n", GetTickCount() - t);

		t = GetTickCount();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLES, (GLuint)(_b->ibDataPointer - _b->indexData), GL_UNSIGNED_INT, 0);

		//printf("draw: %u\n", GetTickCount() - t);

		{
			int i = _b->numAttributes;
			while(i--)
			{
				glDisableVertexAttribArray(_b->attributes[i].location);
			}
		}



		glUseProgram(0);
		
		return 0;
	}

	int drawAllBatches()
	{
		int len = batchList.size();
		int i = 0;
		while(i < len)
		{
			Batch* b = batchList[i];

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