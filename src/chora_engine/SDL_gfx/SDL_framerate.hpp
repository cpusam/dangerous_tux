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

#include "../sdl.hpp"

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
		FPSManager (  )
		{
			time_passed = 0;
			init_framerate();
		}
		
		void init_framerate (  )
		{
			lastticks = baseticks = get_ticks();
		}
		
	public:
		~FPSManager (  )
		{
			if (singleton)
				delete singleton;
		}
		
		static FPSManager * instance (  )
		{
			if (!singleton)
				singleton = new FPSManager();
			
			return singleton;
		}
		
		Uint32 get_ticks (  )
		{
			Uint32 ticks = SDL_GetTicks();

			/* 
			* Since baseticks!=0 is used to track initialization
			* we need to ensure that the tick count is always >0 
			* since SDL_GetTicks may not have incremented yet and
			* return 0 depending on the timing of the calls.
			*/
			if (ticks == 0) {
				return 1;
			} else {
				return ticks;
			}
		}
		
		int set_framerate ( Uint32 rate )
		{
			if ((rate >= FPS_LOWER_LIMIT) && (rate <= FPS_UPPER_LIMIT)) {
				fpsdef.framecount = 0;
				fpsdef.rate = rate;
				fpsdef.rateticks = (1000.0f / (float) rate);
				return (0);
			}

			return (-1);
		}
		
		FPSDef get_fpsdef (  )
		{
			return fpsdef;
		}
		
		bool set_fpsdef ( FPSDef d )
		{
			if (set_framerate(d.rate) > -1)
			{
				fpsdef = d;
				return true;
			}
			
			return false;
		}
		
		Uint32 get_delta (  )
		{
			return time_passed;
		}

		Uint32 update (  )
		{
			Uint32 current_ticks;
			Uint32 target_ticks;
			Uint32 the_delay;
			Uint32 delta_time = 0;

			/*
			* Initialize uninitialized manager 
			*/
			if (baseticks == 0) {
				init_framerate();
			}

			/*
			* Next frame 
			*/
			fpsdef.framecount++;

			/*
			* Get/calc ticks 
			*/
			current_ticks = get_ticks();
			delta_time = current_ticks - lastticks;
			lastticks = current_ticks;
			target_ticks = baseticks + (Uint32) ((float) fpsdef.framecount * fpsdef.rateticks);

			if (current_ticks <= target_ticks) {
				the_delay = target_ticks - current_ticks;
				#ifndef __clang__
					SDL_Delay(the_delay);
				#endif
			} else {
				fpsdef.framecount = 0;
				baseticks = get_ticks();
			}
			
			time_passed = delta_time;
			
			return time_passed;
		}
};

#endif				/* _SDL_framerate_h */

