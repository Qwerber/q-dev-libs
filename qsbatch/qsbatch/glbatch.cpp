#include "stdafx.h"

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <assert.h>

namespace qsb
{

	static SDL_Window* window;
	static SDL_GLContext context;
	static int screenWidth;
	static int screenHeight;

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


#pragma region "batch"
	struct Batch
	{
	
		GLuint shaderProgram;
		GLint vertexLocation;
	
	};

	Batch createBatch()
	{
		
		Batch batch;
		return batch;
	
	}

	void batch_setProgram(Batch* _batch, GLuint _program)
	{
		
		_batch->shaderProgram = _program;
	
	}

	bool batch_setVertextLocation(Batch* _batch, char* vertextAttribName)
	{
		
		if(!(_batch->shaderProgram)) 
		{
			printf("No program set\N");
		}

		_batch->vertexLocation = glGetAttribLocation(_batch->shaderProgram, vertextAttribName);
		
		if(_batch->vertexLocation == -1)
		{
			printf("Not a valid variable name\n");
			return false;
		}
		
		return true;
	
	}
#pragma endregion

	int initGLBatch(int _screenWidth, int _screenHeight, int _defaultColor, SDL_Window* _window)
	{
		
		// store values
		screenWidth = screenWidth;
		screenHeight = _screenHeight;
		window = _window;

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

		// set clear color
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

		return 1;
	
	}

	GLuint createShader(GLuint shaderType, const GLchar* shaderCode)
	{
		
		// create and compile shader
		GLuint shader = glCreateShader(shaderType);
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