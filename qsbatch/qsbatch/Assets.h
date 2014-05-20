#pragma once

#include "stdafx.h"
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_opengl.h>

namespace qass
{

	SDL_Surface* loadSurface(std::string _path);
	GLuint loadTexture(std::string _path);

}