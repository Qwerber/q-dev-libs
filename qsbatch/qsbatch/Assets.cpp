#include "stdafx.h"
#include "Assets.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <string>
#include <stdio.h>
#include <unordered_map>

namespace qass
{
	
	static std::unordered_map<std::string, SDL_Surface*> surfaces;
	static std::unordered_map<std::string, GLuint> openGLTextures;

	// lazyfoo code
	SDL_Surface* loadSurface( std::string _path )
	{
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load( _path.c_str() );
		if( loadedSurface == NULL )
		{
			printf( "Unable to load image %s! SDL_image Error: %s\n", _path.c_str(), IMG_GetError() );
		}
	
		surfaces[_path] = loadedSurface;

		return loadedSurface;
	}
	
	//errors: did you init gl first? did you enable tex2d?
	GLuint loadTexture(std::string _path)
	{
		SDL_Surface* surface = loadSurface(_path);
		GLuint ret;
		glGenTextures(1, &ret);
		glBindTexture(GL_TEXTURE_2D, ret);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 448, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		printf("err 2: %d", glGetError());
		
		SDL_FreeSurface(surface);
		surfaces[_path]	= 0;
		openGLTextures[_path] = ret;

		GLint i = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &i);
		printf("twidth: %d", i);

		return ret;
	}
}