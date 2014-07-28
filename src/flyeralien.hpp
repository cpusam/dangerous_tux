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
		SVect init_pos;
		SVect c_point; // centro da animação
		CPlayer * player;
		CTileMap * map;
		CAnimation * curr_anim;
		vector <CAnimation> anim;
		

	public:
		CGun gun;
		SAlienConfig config;
		
	public:
		#ifndef USE_SDL2
			CFlyerAlien ( CPlayer * p, int m_d, SVect i_p );
		#else
			CFlyerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, SVect i_p );
		#endif
		
		virtual ~CFlyerAlien (  );
		
		void set_map ( CTileMap * m );
		
		void reset (  );
		
		void kill (  );
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
		
		void process (  );
		
		int update (  );
};

#endif

