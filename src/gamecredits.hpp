#ifndef GAMECREDITS_HPP
#define GAMECREDITS_HPP

#include "chora_engine/chora.hpp"
#include "chora_engine/vect.hpp"
#include "chora_engine/util.hpp"
#include "chora_engine/animation.hpp"
#include "chora_engine/background.hpp"
#include "chora_engine/gui/writer.hpp"

enum EGameCreditsState
{
	INACTIVE_CREDITS,
	ACTIVE_CREDITS
};


class CGameCredits: public CStateMachine
{
	protected:
		//CWidget widget;
		CAnimation anim; // apenas para contar o tempo
		CAnimation tux_anim;
		SVect tux_pos;
		SVect bg_pos;
		CBackground bg;
		CCamera * cam;
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

