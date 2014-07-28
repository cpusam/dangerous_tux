/*
	Funcionamento:
	'A': pega a esquerda do alien e direita do tile
	'B': pega abaixo do alien e acima do tile
	'C': pega a direita do alien e a esquerda do tile
	'D': pega acima do alien e abaixo do tile
	Mapa: T = tile sólido
		  'B'
		'C'T'A'
		  'D'
*/

#ifndef CIRCLEALIEN_HPP
#define CIRCLEALIEN_HPP

#include "gameentity.hpp"
#include "aliensdef.hpp"
#include "player.hpp"

class CCircleAlien: public CGameEntity
{
	protected:
		int dir, dir_x, dir_y;
		bool touched_player;
		int tile;
		SVect init_pos;
		std::vector <SVect> c_point;
		CPlayer * player;
		std::vector <int> coll_tiles; // tiles de colisão inteira
		CTileMap * map;
		CAnimation * curr_anim;
		std::vector <CAnimation> anim;
	public:
		SAlienConfig config;
	
	public:
		// get é em que lado do tile o alien está inicialmente pregado, get é a direção
		#ifndef USE_SDL2
			CCircleAlien ( CPlayer * p, SVect i_p, int t );
		#else
			CCircleAlien ( SDL_Renderer * r, CPlayer * p, SVect i_p, int t );
		#endif
		
		virtual ~CCircleAlien (  );
		
		void reset_pos (  );	
		
		void reset (  );
		
		void input ( SDL_Event & event );
		
		void process (  );
		
		int collision_hor (  );
		
		int collision_ver (  );
	
		bool has_coll_tile ( int t );
		
		bool ground_right (  );
		
		bool ground_left (  );
		
		bool ground_down (  );
		
		bool ground_up (  );
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
		
		int update (  );
};

#endif

