/*

SDL_framerate.h: framerate manager

Copyright (C) 2001-2012  Andreas Schiffler

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

Andreas Schiffler -- aschiffler at ferzkopp dot net

*/

#ifndef CHORA_SDL_framerate_h
#define CHORA_SDL_framerate_h

#include "sdl.hpp"

/* --------- Definitions */

/*!
\brief Highest possible rate supported by framerate controller in Hz (1/s).
*/
#define FPS_UPPER_LIMIT		200

/*!
\brief Lowest possible rate supported by framerate controller in Hz (1/s).
*/
#define FPS_LOWER_LIMIT		1

/*!
\brief Default rate of framerate controller in Hz (1/s).
*/
#define FPS_DEFAULT		30

/*!
\brief Structure holding the state and timing information of the framerate controller.
*/
struct FPSDef
{
	Uint32 framecount;
	float rateticks;
	Uint32 rate;

	FPSDef ( Uint32 fc=0, float rt=(1000.0f / (float) FPS_DEFAULT), Uint32 r=FPS_DEFAULT )
	{
		framecount = fc;
		rateticks = rt;
		rate = r;
	}
};

class FPSManager
{
	protected:
		FPSDef fpsdef;
		Uint32 baseticks;
		Uint32 lastticks;
		Uint32 time_passed;
	private:
		static FPSManager * singleton;

	protected:
		FPSManager (  );

		void init_framerate (  );

	public:
		static FPSManager * instance (  );

		static void destroy (  );

		Uint32 get_ticks (  );

		int set_framerate ( Uint32 rate );

		FPSDef get_fpsdef (  );

		bool set_fpsdef ( FPSDef d );

		Uint32 get_delta (  );
		
		double get_delta_sec (  );

		Uint32 update (  );
};

#endif				/* _SDL_framerate_h */

