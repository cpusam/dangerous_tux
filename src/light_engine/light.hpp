#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>


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
// #include "gui/gamemenu.hpp"

// módulo de som
#include "sound/soundplayer.hpp"

#endif

