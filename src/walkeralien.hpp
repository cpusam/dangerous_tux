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
		SVect init_pos;
		vector <int> coll_tiles;
		vector <SVect> c_point;
		CTileMap * map;
		public:
		CAnimation * curr_anim;
		vector <CAnimation> anim;

	public:		
		CGun gun;
		SAlienConfig config;
		
	public:
		#ifndef USE_SDL2
			CWalkerAlien ( CPlayer * p, int m_d, SVect i_p );
		#else
			CWalkerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, SVect i_p );
		#endif
		
		virtual ~CWalkerAlien (  );
		
		void set_map ( CTileMap * m );
		
		void reset (  );
		
		int collide_hor (  );
		
		int collide_ver (  );
		
		bool has_coll_tile ( int tile );
		
		int no_ground (  );
		
		int wall (  );
		
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

