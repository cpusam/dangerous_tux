#ifndef GAMECREDITS_HPP
#define GAMECREDITS_HPP

#include "Chora.hpp"
#include "vect.hpp"
#include "util.hpp"
#include "animation.hpp"
#include "background.hpp"
#include "gui/writer.hpp"

enum EGameCreditsState
{
	INACTIVE_CREDITS,
	ACTIVE_CREDITS
};


class CGameCredits: public StateMachine
{
	protected:
		//Widget widget;
		Animation anim; // apenas para contar o tempo
		Animation tux_anim;
		Vect tux_pos;
		Vect bg_pos;
		Background bg;
		Camera * cam;
		int chosed;
		
	public:
		#ifndef USE_SDL2
			CGameCredits (  );
		#else
			CGameCredits ( SDL_Renderer * r );
		#endif
		
		virtual ~CGameCredits (  );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
		void reset (  );
		
		int update (  );
};

#endif

