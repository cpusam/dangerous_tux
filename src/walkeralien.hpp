#ifndef WALKERALIEN_HPP
#define WALKERALIEN_HPP

#include "aliensdef.hpp"

class CWalkerAlien: public CGameEntity
{
	protected:
		CPlayer * player;
		bool shot_at; // se foi baleado pelo jogador
		bool touched_player; // se tocou o jogador
		int dir; // direção da animação
		float move_distance; // distância de movimento, ou raio de movimento
		Vect init_pos;
		vector <int> coll_tiles;
		vector <Vect> c_point;
		TileMap * map;
		public:
		Animation * curr_anim;
		vector <Animation> anim;

	public:		
		CGun gun;
		SAlienConfig config;
		
	public:
		CWalkerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, Vect i_p );
		
		virtual ~CWalkerAlien (  );
		
		void set_map ( TileMap * m );
		
		void reset (  );
		
		int collide_hor (  );
		
		int collide_ver (  );
		
		bool has_coll_tile ( int tile );
		
		int no_ground (  );
		
		int wall (  );
		
		void kill (  );
		
		void draw ( Camera * cam, SDL_Renderer * renderer );
		
		void process (  );
		
		int update (  );
};

#endif

