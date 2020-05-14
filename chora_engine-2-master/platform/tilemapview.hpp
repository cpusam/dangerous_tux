#ifndef CHORA_TILEMAPVIEW_HPP
#define CHORA_TILEMAPVIEW_HPP

#include "../include/animation.hpp"
#include "tilemap.hpp"

#define OLD_ANIMATED

class CAnimatedTile: public Animation
{
	protected:
		int tile;

	public:
		CAnimatedTile (  )
		{
			tile = -1;
		}

		SDL_Texture * get_texture (  )
		{
			return get_curr_frame().get_texture();
		}

		int get_tile (  )
		{
			return tile;
		}

		void set_tile ( int t )
		{
			tile = t;
		}
};

class TileMapView: public TileMap
{
	private:
		std::vector<int> animatedTilesID;
		std::vector<CAnimatedTile> animatedTiles;
	protected:
		std::map <int, SDL_Rect> source; // par <tile, rect_source>
		std::map <int, CAnimatedTile> animation; // par <tile, animação de tile>
	public:
		SDL_Texture * texture;

	public:
		TileMapView ( int tilesize, SDL_Texture * t=nullptr ): TileMap(tilesize)
		{
			texture = t;
		}
		~TileMapView (  )
		{
		}
		
		CAnimatedTile get_animation ( int tile );
		SDL_Rect get_source ( int tile );
		
		void clear_source (  )
		{
			source.clear();
		}

		void set_source ( int k, SDL_Rect f )
		{
			source[k] = f;
		}

		void add_animation ( CAnimatedTile & a, int t );

		bool is_animated ( int t );

		void update_animation (  );

		// desenha o mapa na posição 0,0
		int draw ( SDL_Renderer * renderer, Camera * cam );
		// desenha o mapa numa posição e usando uma camera
		int draw ( SDL_Renderer * renderer, Camera * cam, int x, int y );
		// desenha todo o mapa numa posição
		int draw ( SDL_Renderer * renderer, int x, int y );
};

#endif
