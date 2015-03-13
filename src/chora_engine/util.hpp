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

/*
	funções de uso geral
*/
#ifndef CHORA_UTIL_HPP
#define CHORA_UTIL_HPP

#include "sdl.hpp"
#include "camera.hpp"

extern "C"
{
	extern Uint32 get_pixel ( SDL_Surface * surface, int x, int y );
	extern void put_pixel ( SDL_Surface *surface, int x, int y, Uint32 pixel );

#ifndef USE_SDL2
	extern SDL_Surface * optimize_surface ( SDL_Surface * s );

	extern SDL_Surface * optimize_surface_alpha ( SDL_Surface * s );

	extern void fill_rect ( CCamera * cam, SDL_Surface * screen, Uint32 color, SDL_Rect r );

	extern void draw_surface ( SDL_Surface * surface, int x, int y, CCamera * cam, SDL_Surface * screen );

	extern SDL_Surface * clone_surface ( SDL_Surface * surf, SDL_Rect src );

	#define FLIP_HOR 0
	#define FLIP_VER 1

	extern SDL_Surface * mirror_surface ( SDL_Surface * surface, int flip=FLIP_HOR );
#else
	extern SDL_Surface * optimize_surface ( SDL_Surface * s, SDL_Surface * screen );

	extern int texture_width ( SDL_Texture * t );
	
	extern int texture_height ( SDL_Texture * t );

	extern void fill_rect ( CCamera * cam, SDL_Renderer * renderer, Uint32 color, SDL_Rect r );

	extern void draw_texture ( SDL_Texture * texture, int x, int y, CCamera * cam, SDL_Renderer * renderer );
#endif
};

#endif

