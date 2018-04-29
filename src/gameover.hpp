#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include "Chora.hpp"
#include "vect.hpp"
#include "util.hpp"
#include "camera.hpp"
#include "animation.hpp"


class CGameOver: public StateMachine
{
	protected:
		#ifndef USE_SDL2
			SDL_Surface * background;
			SDL_Surface * title;
		#else
			SDL_Texture * background;
			SDL_Texture * title;
		#endif
		Animation stars;
		
		Vect t_pos;
		Vect s_pos;
	
	public:
		#ifndef USE_SDL2
			CGameOver (  );
		#else
			CGameOver ( SDL_Renderer * r );
		#endif
		
		virtual ~CGameOver (  );
		
		void reset (  );
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( Camera * cam, SDL_Surface * screen );
		#else
			void draw ( Camera * cam, SDL_Renderer * renderer );
		#endif
};

#endif
