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

#define NO_THREAD_SAFE

#include "sdl.hpp"
#include "include/statemachine.hpp"
#include "include/vect.hpp"
#include "include/camera.hpp"
#include "include/collision.hpp"
#include "include/animation.hpp"
#include "include/sprite.hpp"
#include "include/movable.hpp"
#include "include/util.hpp"
#include "include/background.hpp"
#include "include/texturer.hpp"
#include "include/Exception.hpp"
#include "SDL_gfx/SDL_framerate.hpp"

// módulo de tiles
#include "platform/tilemap.hpp"
#include "platform/tilemapview.hpp"

// módulo de gui
#include "gui/widget.hpp"
#include "gui/label.hpp"
#include "gui/button.hpp"
#include "gui/bar.hpp"
#include "gui/writer.hpp"
#include "gui/textinput.hpp"

// módulo de som
#include "sound/soundplayer.hpp"

//módulo game
#include "game/Entity.hpp"
#include "game/Elements.hpp"
#include "game/Input.hpp"
#include "game/Key.hpp"

#endif

