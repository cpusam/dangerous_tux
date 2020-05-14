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

#ifndef CHORA_TEXTURER_HPP
#define CHORA_TEXTURER_HPP

#include <vector>
#include "textureid.hpp"


class Texturer{
	std::vector<TextureID>	 textureID;
	static Texturer * singleton;

	Texturer();
	virtual ~Texturer();
	
	void destroyAll (  );
	void addTexture ( SDL_Texture *tex, std::string name );
	SDL_Texture * addTexture (SDL_Renderer *renderer, std::string path);
	SDL_Texture * addTexture (SDL_Renderer * renderer, std::string path, SDL_Color colorKey);
	void remTexture ( std::string name );
	SDL_Texture* getTexture ( std::string name, bool throwOnError=true );
	
	public:
		static Texturer *instance();
		
		static SDL_Texture * add (SDL_Renderer * renderer, std::string path);
		static SDL_Texture * add (SDL_Renderer * renderer, std::string path, SDL_Color colorKey );
		static void add (SDL_Texture *tex, std::string name );
		static void rem ( std::string name );
		static SDL_Texture* get ( std::string name, bool throwOnError=true );
		std::vector<TextureID> getTextureID();
		static void destroy();
};

#endif // CHORA_TEXTUREMANAGER_H
