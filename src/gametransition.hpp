#ifndef GAMETRANSITION_HPP
#define GAMETRANSITION_HPP

#include "player.hpp"

#include "background.hpp"
#include "camera.hpp"
#include "platform/tilemap.hpp"
#include "gui/label.hpp"

class CGameTransition: public StateMachine
{
	protected:
		Background bg;
		Camera * cam;
		TileMapView * map;
		CPlayer * player;
		GuiLabel * phrase;
		SDL_Renderer * renderer;
	
	public:
		CGameTransition ( SDL_Renderer * r );

		virtual ~CGameTransition (  );

		void set_cam ( Camera * c, SDL_Renderer * renderer );
		
		void set_bg ( string path );
		
		void set_player ( CPlayer * p );
		
		void reset ( int curr_level, int num_levels );

		void draw (  );
		
		int update (  );
};

#endif

