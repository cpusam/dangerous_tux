/*
 Copyright (C) 2014-2018 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#ifndef CHORA_TEXTUREID_HPP
#define CHORA_TEXTUREID_HPP

#include "sdl.hpp"
#include <iostream>
#include <string>

struct TextureID{
	SDL_Texture *texture;
	std::string path;
	std::string name;

	TextureID();
	TextureID(SDL_Texture *tex, std::string n);
	TextureID(std::string path, SDL_Renderer *renderer);
	TextureID(std::string path, SDL_Renderer *renderer, SDL_Color colorKey);
	
	virtual ~TextureID();
	virtual void destroy();
};

#endif
 
