/*
 Copyright (C) 2014 Samuel Leonardo

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

#ifndef CHORA_WRITER_HPP
#define CHORA_WRITER_HPP

#include <string>
#include <vector>
#include <map>

#include "../sdl.hpp"
#include "../include/util.hpp"
#include "../include/Exception.hpp"

struct SText
{
	std::string text;
	SDL_Color color;
};

enum ETextType
{
	SOLID_TEXT,
	UTF8_TEXT,
	UNICODE_TEXT
};

struct Font
{
	int size;
	std::string path;
	std::string name;
	TTF_Font * font;
	
	Font (  )
	{
		size = 0;
		font = nullptr;
	}
};

/*
	classe para escrever linha de texto com a SDL_ttf
*/
class Writer
{
	private:
		int size; // tamanho da fonte
		std::string path; // caminho para a fonte
		std::map<std::string, Font> fonts;
		SDL_Renderer * renderer;
		static Writer * singleton;

		Writer (  );

	public:
		static Writer * instance (  );
		
		virtual ~Writer (  );

		void free_fonts (  );
		
		int load_font ( std::string path, std::string name, int s );

		static void destroy (  );

		TTF_Font * get_font ( std::string name );

		bool resize_font ( std::string name, int s );

		void set_renderer ( SDL_Renderer * r );

		SDL_Renderer * get_renderer (  );

		SDL_Texture * render_text ( std::string name, std::string text, SDL_Color c, int type=SOLID_TEXT );

		SDL_Surface * render_text_surface ( std::string name, std::string text, SDL_Color c, int type=SOLID_TEXT );
};

#endif

