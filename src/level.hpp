#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <sstream>
#include <string.h>

#include "player.hpp"
#include "walkeralien.hpp"
#include "flyeralien.hpp"
#include "circlealien.hpp"
#include "gamesignal.hpp"

#include "chora_engine/background.hpp"
#include "chora_engine/gui/bar.hpp"
#include "chora_engine/gui/label.hpp"
#include "chora_engine/gui/writer.hpp"

enum ELevelState
{
	INIT_LEVEL,
	RUNNING_LEVEL,
	NEXT_LEVEL
};

class CLevel: public CStateMachine
{
	private:
		int id;
		CTileMapView * map;
		CBackground * bg;
		string bg_path;
		string level_path;
		vector <SVect> jetpacks; // posição de cada jetpack no mapa

	public:
		vector <CGameEntity *> aliens;
		CKernelSignal kernel_signal;
		CExitSignal exit_signal;
		SDL_Color widget_color;
		static CWidget * widget; // os widgets do level
		static CPlayer * player;
		#ifndef USE_SDL2
			static SDL_Surface * screen;
		#endif
		static CCamera * cam;
		#if USE_SDL2
			static SDL_Renderer * renderer;
		#endif

	public:
		#ifndef USE_SDL2
			CLevel ( int tilesize, int i );
		#else
			CLevel ( SDL_Renderer * r, int tilesize, int i ): kernel_signal(r), exit_signal(r
		#endif
		
		virtual ~CLevel (  );
		
		void delete_widget (  );
			
		void unload (  );
		
		string get_bg_path (  );
		
		string get_level_path (  );
		
		CTileMapView * get_map (  );
		
		int get_id (  );

		void draw (  );
	
		void input ( SDL_Event & event );
	
		int update (  );
};


#endif

