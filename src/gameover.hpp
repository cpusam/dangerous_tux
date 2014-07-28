#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include "chora_engine/chora.hpp"
#include "chora_engine/vect.hpp"
#include "chora_engine/util.hpp"
#include "chora_engine/camera.hpp"
#include "chora_engine/animation.hpp"


class CGameOver: public CStateMachine
{
	protected:
		#ifndef USE_SDL2
			SDL_Surface * background;
			SDL_Surface * title;
		#else
			SDL_Texture * background;
			SDL_Texture * title;
		#endif
		CAnimation stars;
		
		SVect t_pos;
		SVect s_pos;
	
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
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
};

#endif
