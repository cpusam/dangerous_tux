#ifndef FLYERALIEN_HPP
#define FLYERALIEN_HPP

#include "aliensdef.hpp"

class CFlyerAlien: public CGameEntity
{
	protected:
		int dir;
		int shot_dir;
		bool shot_at;
		bool touched_player;
		float move_distance; // distância de movimento, ou raio de movimento
		float count, step; // para mover o alien
		Vect init_pos;
		Vect c_point; // centro da animação
		CPlayer * player;
		TileMap * map;
		Animation * curr_anim;
		vector <Animation> anim;
		

	public:
		CGun gun;
		SAlienConfig config;
		
	public:
		CFlyerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, Vect i_p );

		virtual ~CFlyerAlien (  );
		
		void set_map ( TileMap * m );
		
		void reset (  );
		
		void kill (  );
		
		void draw ( Camera * cam, SDL_Renderer * renderer );

		void process (  );
		
		int update (  );
};

#endif

