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

#ifndef CHORA_HPP
#define CHORA_HPP

#if _WIN32 || _WIN64
	#if __MINGW32__ || __MINGW64__
		#ifndef USE_SDL2
			#include <SDL/SDL.h>
			
			#ifndef CHORA_NO_GUI
				#include <SDL/SDL_ttf.h>
			#endif
			
			#ifndef CHORA_NO_SOUND
				#include <SDL/SDL_mixer.h>
			#endif
			
			#ifndef CHORA_NO_IMAGE
				#include <SDL/SDL_image.h>
			#endif
		#else
			#include <SDL2/SDL.h>
			
			#ifndef CHORA_NO_GUI
				#include <SDL2/SDL_ttf.h>
			#endif
			
			#ifndef CHORA_NO_SOUND
				#include <SDL2/SDL_mixer.h>
			#endif
			
			#ifndef CHORA_NO_IMAGE
				#include <SDL2/SDL_image.h>
			#endif
		#endif
	#else
		#include "SDL.h"
		
		#ifndef CHORA_NO_GUI
			#include "SDL_ttf.h"
		#endif
		
		#ifndef CHORA_NO_SOUND
			#include "SDL_mixer.h"
		#endif
		
		#ifndef CHORA_NO_IMAGE
			#include "SDL_image.h"
		#endif
	#endif
	#undef main
#else
	#ifndef USE_SDL2
		#include <SDL/SDL.h>
		#ifndef CHORA_NO_GUI
			#include <SDL/SDL_ttf.h>
		#endif
		#ifndef CHORA_NO_SOUND
			#include <SDL/SDL_mixer.h>
		#endif
		#ifndef CHORA_NO_IMAGE
			#include <SDL/SDL_image.h>
		#endif
	#else
		#include <SDL2/SDL.h>
		#ifndef CHORA_NO_GUI
			#include <SDL2/SDL_ttf.h>
		#endif
		#ifndef CHORA_NO_SOUND
			#include <SDL2/SDL_mixer.h>
		#endif
		#ifndef CHORA_NO_IMAGE
			#include <SDL2/SDL_image.h>
		#endif
	#endif
	#undef main
#endif

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

using namespace std;

#include "statemachine.hpp"
#include "vect.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "animation.hpp"
#include "movable.hpp"
#include "util.hpp"
#include "SDL_gfx/SDL_framerate.hpp"
#include "background.hpp"
#ifndef CHORA_NO_TILE
	// módulo de tiles
	#include "tile/tilemap.hpp"
#endif


#ifndef CHORA_NO_GUI
	// módulo de gui
	#include "gui/writer.hpp"
	#include "gui/widget.hpp"
	#include "gui/label.hpp"
	#include "gui/button.hpp"
	#include "gui/bar.hpp"
#endif

#ifndef CHORA_NO_SOUND
	// módulo de som
	#include "sound/soundplayer.hpp"
#endif
#endif

