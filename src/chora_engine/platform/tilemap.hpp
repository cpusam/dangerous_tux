/*
 Copyright (C) 2014 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#ifndef CHORA_TILEMAP_HPP
#define CHORA_TILEMAP_HPP

#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include "../sdl.hpp"
#include "../vect.hpp"
#include "../camera.hpp"
#include "../animation.hpp"

class CTileMap
{
	protected:
		int width; // largura do std::mapa
		int height;
		int tilesize;
		SVect position;
		SDL_Rect dimension;
		std::vector <int> tileset;
		std::vector <int> tiles;
	
	public:
		CTileMap ( int ts )
		{
			width = height = ts;
			tilesize = ts;
			dimension = (SDL_Rect){0,0,tilesize,tilesize};
		}
		
		void set_tilesize ( int ts );
		
		bool set_tile ( int x, int y, int t );
		
		void set_tile ( int i, int t );
		
		int get_tile ( int x, int y );

		int get_tile ( int i );
		
		int get_tilesize (  );
		
		int get_width (  );
		
		int get_height (  );
		
		void set_position ( SVect p );
		
		SVect get_position (  );
		
		SDL_Rect get_dimension (  );
		
		bool has_tile ( int t );
		
		bool add_tile ( int t );
		
		void remove_tile ( int t );
		
		/*
			Lẽ todo o vetor e se encontra um -1 soma uma linha
		*/
		int read ( std::vector <int> & t );
		/*
			cada linha de tileset está dividida com -1, ou seja, 
			a cada tile sendo -1 é uma linha.
		*/
		int read ( int * t, int size );
		
		/*
			Procurar ler um std::mapa melhor que isso.
		*/
		int read ( char * filename );
};

class CAnimatedTile: public CAnimation
{
	protected:
		int tile;
		
	public:
		CAnimatedTile (  )
		{
			tile = -1;
			#ifndef USE_SDL2
				surface.push_back(NULL);
			#else
				texture.push_back(NULL);
			#endif
		}
		
		#ifndef USE_SDL2
			SDL_Surface * get_surface (  )
			{
				return surface[0];
			}
		
			void set_surface ( SDL_Surface * s )
			{
				if (surface[0] && surface[0] != s)
					SDL_FreeSurface(surface[0]);

				surface[0] = s;
			}
		#else
			SDL_Texture * get_texture (  )
			{
				return texture[0];
			}
			
			void set_texture ( SDL_Texture * t )
			{
				if (texture[0] && texture[0] != t)
					SDL_DestroyTexture(texture[0]);
					
				texture[0] = t;
			}
		#endif
		
		int get_tile (  )
		{
			return tile;
		}
		
		void set_tile ( int t )
		{
			tile = t;
		}
};

class CTileMapView: public CTileMap
{
	protected:
		std::map <int, SDL_Rect> source; // par <tile, rect_source>
		std::map <int, CAnimatedTile> animation; // par <tile, animação de tile>
	public:
		#ifndef USE_SDL2
			SDL_Surface * surface;
		#else
			SDL_Texture * texture;
		#endif
	
	public:
		#ifndef USE_SDL2
			CTileMapView ( int tilesize, SDL_Surface * s=0 ): CTileMap(tilesize)
			{
				surface = s;
			}
		#else
			CTileMapView ( int tilesize, SDL_Texture * t=0 ): CTileMap(tilesize)
			{
				texture = t;
			}
		#endif

		~CTileMapView (  )
		{
			#ifndef USE_SDL2
			if (surface)
				SDL_FreeSurface(surface);
			#else
				if (texture)
					SDL_DestroyTexture(texture);
			#endif
		}
		
		void clear_source (  )
		{
			source.clear();
		}
	
		void set_source ( int k, SDL_Rect f )
		{
			source[k] = f;
		}
		
		void add_animation ( CAnimatedTile a, int t );
		
		bool is_animated (	int t );
		
		void update_animation (  );
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
};



#endif


