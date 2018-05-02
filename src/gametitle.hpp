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
		SDL_Texture * background;
		SDL_Texture * title_name;

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
		CGameTitle ( SDL_Renderer * r );

		virtual ~CGameTitle (  );
		
		void reset (  );
		
		void input ( SDL_Event & event );
		
		int update (  );
		void draw ( SDL_Renderer * renderer );
};

#endif
