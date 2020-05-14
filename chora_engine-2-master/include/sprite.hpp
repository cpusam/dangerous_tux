#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>

#include "sdl.hpp"
#include "animation.hpp"
#include "vect.hpp"

/*
	Trocar retangulos por shapes isso dá maior possibilidade de fazer mais coisas
	Tipo, criar uma classe SCollisionShape ao inves de Rect.
	Isso dá a possibilidade de criar formas que tenham rotação como OBBs
*/

enum ECollisionRectType
{
	UNDEFINED_RECT = -1,
	COLLISION_RECT, // é apenas um retangulo de colisão
	SENSOR_RECT, // retangulo sensor
	HIT_RECT, // se é retangulo de receber golpes
	ATTACK_RECT, // se é um retangulo que dá ataques
	DEFENSE_RECT, // se é um retangulo que defende ataques
};

struct SCollisionRect
{
	private:
		int type;
	public:
		int x, y;
		int w, h;

	public:
		SCollisionRect (  );

		SCollisionRect ( int t, SDL_Rect r );

		void set_rect ( int t, SDL_Rect r );

		void set_type ( int t );
		int get_type (  );

		// pega o retangulo em releção a posição p no mundo
		SDL_Rect get_world_rect ( Vect p );
};

class CollisionFrame: public AnimationFrame
{
	protected:
		std::vector <SCollisionRect> rects; // retangulos de colisão

	public:
		CollisionFrame (  )
		{
		}

		CollisionFrame ( int d, SDL_Rect src, std::vector <SCollisionRect> r ): AnimationFrame(d, src)
		{
			rects = r;
		}

		void set_source ( SDL_Rect src );
		void add_rect ( SCollisionRect r );
		bool set_rect ( int i, SCollisionRect r );
		
		void clear_rects (  );
		std::vector <SCollisionRect> get_rects (  );

		SCollisionRect get_rect ( int i );
		std::vector <SCollisionRect> get_rects_type ( int t );
};

class Sprite: public Animation
{
	protected:
		std::vector <CollisionFrame> coll_frames;

	protected:
		using Animation::add_frame;

	public:
		Sprite (  )
		{
		}


		void set_coll_frames ( SDL_Texture *t, std::vector <CollisionFrame> c_f );
		void add_frame ( SDL_Texture * t, CollisionFrame c );
		CollisionFrame get_coll_frame ( int i );
		std::vector <CollisionFrame> get_coll_frames (  );
};

#endif

