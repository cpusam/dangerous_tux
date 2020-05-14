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
#include <string>
#include <fstream>
#include <sstream>

#include "../sdl.hpp"
#include "../include/vect.hpp"
#include "../include/camera.hpp"
#include "../include/animation.hpp"

class TileMap
{
	protected:
		int width; // largura do std::mapa
		int height;
		int tilesize;
		Vect pos;
		SDL_Rect dimension;
		std::vector <int> tileset;//conjunto de tiles do mapa
		std::vector <int> tiles;//tiles neste mapa

	public:
		TileMap ( int ts )
		{
			width = height = 1;
			tilesize = ts;
			dimension = (SDL_Rect){0,0,tilesize,tilesize};
		}

		TileMap ( int ts, int w, int h )
		{
			width = w;
			height = h;
			tilesize = ts;
			dimension = (SDL_Rect){0,0,tilesize * width,tilesize * height};
		}

		void set_tilesize ( int ts );

		void set_pos ( Vect p );
		
		Vect get_pos (  );

		bool set_tile ( int x, int y, int t );

		void set_tile ( int i, int t );

		inline int get_tile ( int x, int y )
		{
			if (x < 0 || y < 0)
			{
				static int p = 0;
				if (p < 100)
				{
					printf("CTilaMap: get_tile posições negativas x = %d, y = %d\n", x, y);
				}
				p++;
				//throw "TileMap: get_tile posições negativas!\n";
				return -1;
			}

			x = int((x - pos.x) / tilesize);
			y = int((y - pos.y) / tilesize);

			if (y >= height || x >= width)
				return -1;

			if (y * width + x < int(tileset.size()))
			{
				//if (x < width && y < height)
				return tileset[y * width + x];
			}

			return -1;
		}
		
		//pega a posição em pixels
		Vect get_tile_pos ( int i );

		// Se o indice i for um tile, retorna >= 0. Caso contrário < 0 caso não haja
		int get_tile ( int i );

		int get_tilesize (  );

		int get_width (  );

		int get_height (  );

		SDL_Rect get_dimension (  );

		bool has_tile ( int t );

		bool add_tile ( int t );

		void remove_tile ( int t );

		int find_first_pos ( int t );

		/*
			Lẽ todo o vetor e se encontra um -1 soma uma linha
		*/
		int read ( std::vector <int> & t );
		/*
			cada linha de tileset está dividida com -1, ou seja,
			a cada tile sendo -1 é uma linha.
		*/
		int read ( int * t, int size );

		int read ( std::string filename );

		//lê um mapa de um xpm
		int read_from_xpm ( std::string filename );
};


#endif


