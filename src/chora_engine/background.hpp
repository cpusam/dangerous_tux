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

#ifndef CHORA_BACKGROUND_HPP
#define CHORA_BACKGROUND_HPP

#include "sdl.hpp"
#include "vect.hpp"
#include "camera.hpp"
#include "animation.hpp"

/*
	NOTA: quando a posição é negativa não está havendo movimento do background
*/

class CBackground
{
	protected:
		#ifndef USE_SDL2
			SDL_Surface * surface;
		#else
			SDL_Texture * texture;
		#endif
	
	public:
		#ifndef USE_SDL2
			CBackground ( SDL_Surface * s=0 )
			{
				surface = s;
			}
		#else
			CBackground ( SDL_Texture * t=0 )
			{
				texture = t;
			}
		#endif
		
		~CBackground (  )
		{
			#ifndef USE_SDL2
				if (surface)
					SDL_FreeSurface(surface);
			#else
				if (texture)
					SDL_DestroyTexture(texture);
			#endif
		}
		
		#ifndef USE_SDL2
			SDL_Surface * get_surface (  );
			
			bool set_surface ( SDL_Surface * s );
		#else
			SDL_Texture * get_texture (  );
			
			bool set_texture ( SDL_Texture * t );
		#endif
		
		/*
			scrolling nos eixos x e y mas limitado pelas bordas da surface
		*/
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
		
		// apenas um scrolling horizontal
		#ifndef USE_SDL2
			void draw_hor ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw_hor ( CCamera * cam, SDL_Renderer * renderer );
		#endif
		
		// NOTE: falta testar, precisa refazer
		// apenas um scrolling vertical
		#ifndef USE_SDL2
			void draw_ver ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw_ver ( CCamera * cam, SDL_Renderer * renderer );
		#endif
};

class CAnimatedBackground: private CStateMachine
{
	protected:
		CAnimation anim[2];

	public:
		CAnimatedBackground (  )
		{
			set_state(1);
		}

		~CAnimatedBackground (  )
		{
			#ifndef USE_SDL2
				anim[0].destroy_surfaces();
			#else
				anim[0].destroy_textures();
			#endif
		}
		
		#ifndef USE_SDL2
			void add_frame ( SDL_Surface * s, SDL_Rect f, int d );
		#else
			void add_frame ( SDL_Texture *t, SDL_Rect f, int d );
		#endif
		
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
		
		#ifndef USE_SDL2
			void draw_hor ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw_hor ( CCamera * cam, SDL_Renderer * renderer );
		#endif
};

#endif

