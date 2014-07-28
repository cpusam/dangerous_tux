#ifndef GAMETITLE_HPP
#define GAMETITLE_HPP

#include "chora_engine/chora.hpp"
#include "chora_engine/animation.hpp"
#include "chora_engine/gui/label.hpp"

enum EGameTitleState
{
	RUNNING_GAMETITLE,
	BACKGROUND_GAMETITLE
};

class CGameTitle: public CStateMachine
{
	protected:
		#ifndef USE_SDL2
			SDL_Surface * background;
			SDL_Surface * title_name;
		#else
			SDL_Texture * background;
			SDL_Texture * title_name;
		#endif
		CAnimation tux_rocket;
		CLabel * press_enter;
		CLabel * version;
		CAnimation p_enter;
		
		SVect tr_pos;
		SVect tn_pos;
		SVect tn_init;
		SVect tn_final;
		float tn_vel;
	
	public:
		#ifndef USE_SDL2
			CGameTitle (  );
		#else
			CGameTitle ( SDL_Renderer * r );
		#endif
		
		virtual ~CGameTitle (  );
		
		void reset (  );
		
		void input ( SDL_Event & event );
		
		int update (  );
	
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
};

#endif
