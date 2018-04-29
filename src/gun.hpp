#ifndef GUN_HPP
#define GUN_HPP

#include "gameentity.hpp"
#include "vect.hpp"
#include "util.hpp"
#include "collision.hpp"
#include "sound/soundplayer.hpp"
#include "platform/tilemap.hpp"

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
		Vect c_point; // ponto de colisão
		TileMap * map; // mapa para colisão
		std::vector <CGameEntity *> target;

	public:
		CShot (  );
		virtual ~CShot (  );

		void set_map ( TileMap * m );

		void set_shot ( Vect p, Vect v );// posição e direção
		
		void add_target ( CGameEntity * t );
		
		void rem_target ( CGameEntity * t );
		
		void clear_targets (  );
		
		int collision (  );
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( Camera * cam, SDL_Surface * screen );
		#else
			void draw ( Camera * cam, SDL_Renderer * renderer );
		#endif
};

class CGun: public StateMachine
{
	protected:
		bool has; // se tem a arma
		bool used; // se foi usada e o tiro ainda está sendo visto
		Vect pos_dir[2]; // posição relativa do jogador para a arma
	public:
		CShot shot;
	
	public:
		CGun ( bool h=false );

		CGun ( Vect p[2], bool h=false );
		
		virtual ~CGun (  );
		
		void set_pos_dir ( Vect p[2] );
		
		void set_gun ( bool h );

		bool has_gun (  );
		
		bool was_used (  );
		
		void fire ( Vect entity_pos, int d, Vect vel_shot );
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( Camera * cam, SDL_Surface * screen );
		#else
			void draw ( Camera * cam, SDL_Renderer * renderer );
		#endif
};

#endif

