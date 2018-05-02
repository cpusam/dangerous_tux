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

#ifndef GYROALIEN_HPP
#define GYROALIEN_HPP

#include "gameentity.hpp"
#include "aliensdef.hpp"
#include "player.hpp"

class CGyroAlien: public CGameEntity
{
	protected:
		int dir, dir_x, dir_y;
		bool touched_player;
		int tile;
		Vect init_pos;
		std::vector <Vect> c_point;
		CPlayer * player;
		std::vector <int> coll_tiles; // tiles de colisão inteira
		TileMap * map;
		Animation * curr_anim;
		std::vector <Animation> anim;
	public:
		SAlienConfig config;
	
	public:
		// get é em que lado do tile o alien está inicialmente pregado, get é a direção
		CGyroAlien ( SDL_Renderer * r, CPlayer * p, Vect i_p, int t );
		
		virtual ~CGyroAlien (  );
		
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
		
		void draw ( Camera * cam, SDL_Renderer * renderer );
		
		int update (  );
};

#endif


