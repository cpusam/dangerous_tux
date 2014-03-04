#ifndef LIGHT_HPP
#define LIGHT_HPP

#ifndef USE_SDL2
	#include <SDL/SDL.h>
	#include <SDL/SDL_ttf.h>
	#include <SDL/SDL_mixer.h>
	#include <SDL/SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_image.h>
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
#include "animation.hpp"
#include "tilemap.hpp"
#include "collision.hpp"
#include "background.hpp"
#include "movable.hpp"
#include "util.hpp"

// módulo de gui
#include "gui/writer.hpp"
#include "gui/widget.hpp"
#include "gui/label.hpp"
#include "gui/button.hpp"
#include "gui/bar.hpp"

// módulo de som
#include "sound/soundplayer.hpp"

#endif

