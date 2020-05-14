/*
 Copyright (C) 2014 Chora Engine (Samuel Leonardo)

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

#ifndef CHORA_SDL_HPP
#define CHORA_SDL_HPP

#if defined(_WIN32) || defined(_WIN64)
		#include "SDL.h"
		#include "SDL_ttf.h"
		#include "SDL_mixer.h"
		#include "SDL_image.h"
		#undef main
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_image.h>
#endif

#endif

