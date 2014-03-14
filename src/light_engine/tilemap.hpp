#ifndef TILEMAP_HPP
#define TILEMAP_HPP

class CTileMap
{
	private:
		int width; // largura do mapa
		int height;
		int tilesize;
		SVect position;
		SDL_Rect dimension;
		vector <int> tileset;
	protected:
		vector <int> tiles;
	
	public:
		CTileMap ( int ts )
		{
			width = height = ts;
			tilesize = ts;
			dimension = (SDL_Rect){0,0,ts,ts};
		}
		
		void set_tilesize ( int ts )
		{
			tilesize = ts;
		}
		
		bool set_tile ( int x, int y, int t )
		{
			x = x / tilesize;
			y = y / tilesize;
			
			int p = y*width + x;
			if (p > tileset.size())
				return false;
			else
				tileset[p] = t;
			
			return true;
		}
		
		void set_tile ( int i, int t )
		{
			if (i > -1 && i < tileset.size())
			{
				tileset[i] = t;
				if (!has_tile(t))
					tiles.push_back(t);
			}
		}
		
		int get_tile ( int x, int y )
		{
			x = x / tilesize;
			y = y / tilesize;
			
			if (y * width + x < tileset.size())
				return tileset[y * width + x];
			
			return -1;
		}

		int get_tile ( int i )
		{
			if (i > -1 && i < tileset.size())
				return tileset[i];
			
			return -1;
		}
		
		int get_tilesize (  )
		{
			return tilesize;
		}
		
		int get_width (  )
		{
			return width;
		}
		
		int get_height (  )
		{
			return height;
		}
		
		void set_position ( SVect p )
		{
			position = p;
			dimension.x = p.x;
			dimension.y = p.y;
		}
		
		SVect get_position (  )
		{
			return position;
		}
		
		SDL_Rect get_dimension (  )
		{
			return dimension;
		}
		
		bool has_tile ( int t )
		{
			for (int i = 0; i < tiles.size(); i++)
				if (t == tiles[i])
					return true;

			return false;
		}
		
		bool add_tile ( int t )
		{
			if (has_tile(t))
				return false;
			
			tiles.push_back(t);
			return true;
		}
		
		void remove_tile ( int t )
		{
			for (vector <int>::iterator it = tiles.begin(); it != tiles.end(); it++)
				if (t == *it)
				{
					tiles.erase(it);
					break;
				}
		}
		
		/*
			Lẽ todo o vetor e se encontra um -1 soma uma linha
		*/
		int read ( vector <int> & t )
		{
			tileset.clear();
			width = height = 0;
			for (vector <int>::iterator i = t.begin(); i < t.end(); i++)
			{
				if (*i <= -1)
				{
					height++;
					if (width == 0)
						width = int(i - t.begin());
				}
				else
				{
					tileset.push_back(*i);
					if (!has_tile(*i))
						tiles.push_back(*i);
				}
			}

			if (t[t.size() - 1] != -1)
				height++;

			dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
			return 1;
		}
		/*
			cada linha de tileset está dividida com -1, ou seja, 
			a cada tile sendo -1 é uma linha.
		*/
		int read ( int * t, int size )
		{
			tileset.clear();
			width = height = 0;
			for (int i(0); i < size; i++)
			{
				if (t[i] == -1)
				{
					height++;
					if (width == 0)
						width = i;
				}
				else if (t[i] >= ' ')
				{
					tileset.push_back(t[i]);
					if (!has_tile(t[i]))
						tiles.push_back(t[i]);
				}
			}

			if (t[size - 1] != -1)
				height++;

			dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
			return 1;
		}
		
		/*
			Procurar ler um mapa melhor que isso.
		*/
		int read ( char * filename )
    {
    	ifstream file(filename, ifstream::in | ifstream::binary);
    	if (!file)
    		return 0;

			tileset.clear();
			width = height = 0;
			while (file)
			{
				char tile;
				file.get(tile);
				
				if (tile == '\n')
				{
					if (width == 0)
					{
						width = tileset.size();
					}
				}
				else if (file && tile >= ' ')
				{
					tileset.push_back(tile);
					if (!has_tile(tile))
						tiles.push_back(tile);
				}
			}
			height = tileset.size() / width;
			dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
			file.close();
			
			return 1;
		}
};

class CAnimatedTile: public CAnimation
{
	protected:
		int tile;
		#ifndef USE_SDL2
			using CAnimation::surface;
		#else
			using CAnimation::texture;
		#endif

	public:
		CAnimatedTile (  )
		{
			tile = -1;
		}
		
		int get_frames_size (  )
		{
			return frames.size();
		}
		
		#ifndef USE_SDL2
			SDL_Surface * get_surface (  )
			{
				return surface;
			}
		
			void set_surface ( SDL_Surface * s )
			{
				if (surface)
					SDL_FreeSurface(surface);

				surface = s;
			}
		#else
			vector <SDL_Texture *> get_texture (  )
			{
				return texture;
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
		map <int, SDL_Rect> source; // par <tile, rect_sourcee>
		map <int, CAnimatedTile> animation;
	public:
		#ifndef USE_SDL2
			SDL_Surface * surface;
		#else
			SDL_Texture * texture;
		#endif
	
	public:
		#ifndef USE_SDL2
			CTileMapView ( int ts, SDL_Surface * s=0 ): CTileMap(ts)
			{
				surface = s;
			}
		#else
			CTileMapView ( int ts, SDL_Texture * t=0 ): CTileMap(ts)
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
		
		void add_animation ( CAnimatedTile a, int t )
		{
			#ifndef USE_SDL2
				a.set_surface(surface);
			#endif
			a.set_tile(t);
			add_tile(t);
			animation[t] = a;
		}
		
		bool is_animated (	int t )
		{
			return (animation.count(t) > 0);
		}
		
		void update_animation (  )
		{
			for (vector <int>::iterator i = tiles.begin(); i != tiles.end(); i++)
			{
				if (is_animated(*i))
					animation[*i].update();
			}
		}
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			int i, j, t;
			static int ts = get_tilesize();
			SVect pos = cam->get_position(), p = cam->get_position();
			SDL_Rect dest ={0,0,ts,ts};
			SDL_Rect src = {0,0,0,0};
			SDL_Rect dim = cam->get_dimension();
			
			pos.x = int(pos.x) / ts;
			pos.y = int(pos.y) / ts;
			dim.w /= ts;
			dim.h /= ts;
			
			for (i = pos.x; i <= pos.x + dim.w; i++)
				for (j = pos.y; j <= pos.y + dim.h; j++)
				{
					t = get_tile(i * ts, j * ts);
					if (!has_tile(t))
						continue;

					if (is_animated(t))
					{
						#ifndef USE_SDL2
							animation[t].draw(i * ts, j * ts, cam, screen);
						#else
							animation[t].draw(i * ts, j * ts, cam, renderer);
						#endif
						continue;
					}
					
					src = source[t];
				
					if (i == pos.x)
					{
						src.x += int(p.x) % ts;
						src.w = ts - int(p.x) % ts;
						dest.x = dim.x;
					}
					else if (i == pos.x + dim.w)
					{
						src.w = int(p.x) % ts;
						dest.x = dim.x + dim.w * ts - src.w;
					}
					else
					{
						dest.x = dim.x + (i - pos.x) * ts - (int(p.x) % ts);
					}
					
					if (j == pos.y)
					{
						src.y += int(p.y) % ts;
						src.h = ts - int(p.y) % ts;
						dest.y = dim.y;
					}
					else if (j == pos.y + dim.h)
					{
						src.h = int(p.y) % ts;
						dest.y = dim.y + dim.h * ts - src.h;
					}
					else
					{
						dest.y = dim.y + (j - pos.y) * ts - (int(p.y) % ts);
					}
					
					#ifndef USE_SDL2
						if (surface)
						{
							dest.w = src.w;
							dest.h = src.h;
							SDL_BlitSurface(surface, &src, screen, &dest);
						}
					#else
						if (texture)
						{
							dest.w = src.w;
							dest.h = src.h;
							SDL_RenderCopy(renderer, texture, &src, &dest);
						}
					#endif
				}
		}
};



#endif


