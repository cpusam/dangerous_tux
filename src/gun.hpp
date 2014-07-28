#ifndef GUN_HPP
#define GUN_HPP

#include "gameentity.hpp"
#include "chora_engine/vect.hpp"
#include "chora_engine/util.hpp"
#include "chora_engine/collision.hpp"
#include "chora_engine/sound/soundplayer.hpp"
#include "chora_engine/platform/tilemap.hpp"

enum CShotType
{
	VER_SHOT, // tiro vertical
	HOR_SHOT, // tiro horizontal
	DIA_SHOT // tiro diagonal
};

class CShot: public CGameEntity
{
	protected:
		int dir; // direção a seguir
		std::vector <int> coll_tiles; // tiles de colisão
		SVect c_point; // ponto de colisão
		CTileMap * map; // mapa para colisão
		std::vector <CGameEntity *> target;

	public:
		CShot (  );
		virtual ~CShot (  );

		void set_map ( CTileMap * m );

		void set_shot ( SVect p, SVect v );// posição e direção
		
		void add_target ( CGameEntity * t );
		
		void rem_target ( CGameEntity * t );
		
		void clear_targets (  );
		
		int collision (  );
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
};

class CGun: public CStateMachine
{
	protected:
		bool has; // se tem a arma
		bool used; // se foi usada e o tiro ainda está sendo visto
		SVect pos_dir[2]; // posição relativa do jogador para a arma
	public:
		CShot shot;
	
	public:
		CGun ( bool h=false );

		CGun ( SVect p[2], bool h=false );
		
		virtual ~CGun (  );
		
		void set_pos_dir ( SVect p[2] );
		
		void set_gun ( bool h );

		bool has_gun (  );
		
		bool was_used (  );
		
		void fire ( SVect entity_pos, int d, SVect vel_shot );
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
};

#endif

