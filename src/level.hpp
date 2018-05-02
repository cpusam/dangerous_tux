#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <sstream>
#include <string.h>

#include "player.hpp"
#include "walkeralien.hpp"
#include "flyeralien.hpp"
#include "gyroalien.hpp"
#include "gamesignal.hpp"

#include "background.hpp"
#include "gui/bar.hpp"
#include "gui/label.hpp"
#include "gui/writer.hpp"

enum ELevelState
{
	INIT_LEVEL,
	RUNNING_LEVEL,
	NEXT_LEVEL
};

class CLevel: public StateMachine
{
	private:
		int id;
		TileMapView * map;
		Background * bg;
		string bg_path;
		string level_path;
		vector <Vect> jetpacks; // posição de cada jetpack no mapa

	public:
		vector <CGameEntity *> aliens;
		CKernelSignal kernel_signal;
		CExitSignal exit_signal;
		SDL_Color widget_color;
		static Widget * widget; // os widgets do level
		static CPlayer * player;
		static Camera * cam;
		static SDL_Renderer * renderer;

	public:
		CLevel ( SDL_Renderer * r, int tilesize, int i );
		
		virtual ~CLevel (  );
		
		void delete_widget (  );
			
		void unload (  );
		
		string get_bg_path (  );
		
		string get_level_path (  );
		
		TileMapView * get_map (  );
		
		int get_id (  );

		void draw (  );
	
		void input ( SDL_Event & event );
	
		int update (  );
};


#endif

