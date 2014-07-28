#ifndef GAMETRANSITION_HPP
#define GAMETRANSITION_HPP

#include "player.hpp"

#include "chora_engine/background.hpp"
#include "chora_engine/camera.hpp"
#include "chora_engine/platform/tilemap.hpp"
#include "chora_engine/gui/label.hpp"

class CGameTransition: public CStateMachine
{
	protected:
		CBackground bg;
		CCamera * cam;
		CTileMapView * map;
		CPlayer * player;
		CLabel * phrase;
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
			void set_cam ( CCamera * c, SDL_Surface * screen );
		#else
			void set_cam ( CCamera * c, SDL_Renderer * renderer );
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

