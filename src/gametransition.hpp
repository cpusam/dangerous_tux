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
		#if USE_SDL2
			SDL_Renderer * renderer;
		#endif
	
	public:
		#ifndef USE_SDL2
			CGameTransition (  );
		#else
			CGameTransition ( SDL_Renderer * r );
		#endif
		
		virtual ~CGameTransition (  );
		
		#ifndef USE_SDL2
			void set_cam ( Camera * c, SDL_Surface * screen );
		#else
			void set_cam ( Camera * c, SDL_Renderer * renderer );
		#endif
		
		void set_bg ( string path );
		
		void set_player ( CPlayer * p );
		
		void reset ( int curr_level, int num_levels );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw (  );
		#endif
		
		int update (  );
};

#endif

