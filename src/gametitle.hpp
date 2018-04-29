#ifndef GAMETITLE_HPP
#define GAMETITLE_HPP

#include "Chora.hpp"
#include "animation.hpp"
#include "gui/label.hpp"

enum EGameTitleState
{
	RUNNING_GAMETITLE,
	BACKGROUND_GAMETITLE
};

class CGameTitle: public StateMachine
{
	protected:
		#ifndef USE_SDL2
			SDL_Surface * background;
			SDL_Surface * title_name;
		#else
			SDL_Texture * background;
			SDL_Texture * title_name;
		#endif
		Animation tux_rocket;
		GuiLabel * press_enter;
		GuiLabel * version;
		Animation p_enter;
		
		Vect tr_pos;
		Vect tn_pos;
		Vect tn_init;
		Vect tn_final;
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
