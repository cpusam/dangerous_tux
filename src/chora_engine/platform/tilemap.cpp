#include "tilemap.hpp"

void CTileMap::set_tilesize ( int ts )
{
	tilesize = ts;
}

bool CTileMap::set_tile ( int x, int y, int t )
{
	if (x < 0 || y < 0)
		throw "CTileMap: get_tile posições negativas!\n";
	
	x = x / tilesize;
	y = y / tilesize;
	
	int p = y*width + x;
	if (p >= tileset.size())
		return false;
	else
		tileset[p] = t;
	
	return true;
}

void CTileMap::set_tile ( int i, int t )
{
	if (i > -1 && i < tileset.size())
	{
		tileset[i] = t;
		if (!has_tile(t))
			tiles.push_back(t);
	}
}

int CTileMap::get_tile ( int x, int y )
{
	if (x < 0 || y < 0)
	{
		printf("CTilaMap: get_tile posições negativas x = %d, y = %d\n", x, y);
		//throw "CTileMap: get_tile posições negativas!\n";
		return -1;
	}
	
	x = int(x / tilesize);
	y = int(y / tilesize);

	if (y * width + x < tileset.size())
	{
		//if (x < width && y < height)
			return tileset.at(y * width + x);
	}
	
	return -1;
}

int CTileMap::get_tile ( int i )
{
	if (i > -1 && i < tileset.size())
		return tileset.at(i);
	
	return -1;
}

int CTileMap::find_first_pos ( int t )
{
	for (int i = 0; i < tileset.size(); i++)
		if (t == tileset[i])
			return i;
	
	return -1; // sem posição ou não encontrado
}

int CTileMap::get_tilesize (  )
{
	return tilesize;
}

int CTileMap::get_width (  )
{
	return width;
}

int CTileMap::get_height (  )
{
	return height;
}

void CTileMap::set_position ( SVect p )
{
	position = p;
	dimension.x = p.x;
	dimension.y = p.y;
}

SVect CTileMap::get_position (  )
{
	return position;
}

SDL_Rect CTileMap::get_dimension (  )
{
	return dimension;
}

bool CTileMap::has_tile ( int t )
{
	for (int i = 0; i < tiles.size(); i++)
		if (t == tiles[i])
			return true;

	return false;
}

bool CTileMap::add_tile ( int t )
{
	if (has_tile(t))
		return false;
	
	tiles.push_back(t);
	return true;
}

void CTileMap::remove_tile ( int t )
{
	for (std::vector <int>::iterator it = tiles.begin(); it != tiles.end(); it++)
		if (t == *it)
		{
			tiles.erase(it);
			break;
		}
}

/*
	Lẽ todo o vetor e se encontra um -1 soma uma linha
*/
int CTileMap::read ( std::vector <int> & t )
{
	tileset.clear();
	width = height = 0;
	for (std::vector <int>::iterator i = t.begin(); i < t.end(); i++)
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

	if (t.back() != -1)
		height++;

	dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
	return 1;
}
/*
	cada linha de tileset está dividida com -1, ou seja, 
	a cada tile sendo -1 é uma linha.
*/
int CTileMap::read ( int * t, int size )
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
int CTileMap::read ( char * filename )
{
	std::ifstream file(filename, std::ifstream::in | std::ifstream::binary);
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


////////////////////////////////////////////////////////////////////
void CTileMapView::add_animation ( CAnimatedTile a, int t )
{
	#ifndef USE_SDL2
		if (!surface)
			throw "CTileMapView: surface nula";
		a.set_surface(surface);
	#else
		if (!texture)
			throw "CTileMapView: texture nula";
		a.set_texture(texture);
	#endif
	
	a.set_tile(t);
	add_tile(t);
	animation[t] = a;
}

bool CTileMapView::is_animated (	int t )
{
	return (animation.count(t) > 0);
}

void CTileMapView::update_animation (  )
{
	for (std::vector <int>::iterator i = tiles.begin(); i != tiles.end(); i++)
	{
		if (is_animated(*i))
			animation[*i].update();
	}
}



#ifndef USE_SDL2
	void CTileMapView::draw ( CCamera * cam, SDL_Surface * screen )
#else
	void CTileMapView::draw ( CCamera * cam, SDL_Renderer * renderer )
#endif
{
	int i, j, t;
	SVect pos = cam->get_position(), p = cam->get_position();
	SDL_Rect dest ={0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = cam->get_dimension();
	
	pos.x = int(pos.x) / tilesize;
	pos.y = int(pos.y) / tilesize;
	dim.w /= tilesize;
	dim.h /= tilesize;
	int mod_x = int(p.x) % tilesize;
	int mod_y = int(p.y) % tilesize;
	
	for (i = pos.x; i <= pos.x + dim.w; i++)
		for (j = pos.y; j <= pos.y + dim.h; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			if (is_animated(t))
			{
				#ifndef USE_SDL2
					animation[t].draw(i * tilesize, j * tilesize, cam, screen);
				#else
					animation[t].draw(i * tilesize, j * tilesize, cam, renderer);
				#endif
				continue;
			}
			
			src = source[t];
		
			if (i == pos.x)
			{
				src.x += mod_x;
				src.w = tilesize - mod_x;
				dest.x = dim.x;
			}
			else if (i == pos.x + dim.w)
			{
				if (mod_x == 0)
					continue;
				
				src.w = mod_x;
				dest.x = dim.x + dim.w * tilesize - src.w;
			}
			else
			{
				dest.x = dim.x + (i - pos.x) * tilesize - (mod_x);
			}
			
			if (j == pos.y)
			{
				src.y += mod_y;
				src.h = tilesize - mod_y;
				dest.y = dim.y;
			}
			else if (j == pos.y + dim.h)
			{
				if (mod_y == 0)
					continue;
				
				src.h = mod_y;
				dest.y = dim.y + dim.h * tilesize - src.h;
			}
			else
			{
				dest.y = dim.y + (j - pos.y) * tilesize - (mod_y);
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

void CTileMapView::draw ( int x, int y, SDL_Rect cam, SDL_Renderer * renderer )
{
	int i, j, t;
	SVect pos = SVect(cam.x, cam.y), p = SVect(cam.x, cam.y);
	SDL_Rect dest ={0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, cam.w, cam.h};


	// limita o movimento da camera
	if (dim.w > width)
		dim.w = width;

	if (dim.h > height)
		dim.h = height;

	if (pos.x < 0)
		pos.x = 0;
	else if (pos.x + dim.w > width)
		pos.x = width - dim.w;

	if (pos.y < 0)
		pos.y = 0;
	else if (pos.y + dim.h > height)
		pos.y = height - dim.h;
	
	
	pos.x = int(pos.x) / tilesize;
	pos.y = int(pos.y) / tilesize;
	dim.w /= tilesize;
	dim.h /= tilesize;
	int mod_x = int(p.x) % tilesize;
	int mod_y = int(p.y) % tilesize;
	


	for (i = pos.x; i <= pos.x + dim.w; i++)
		for (j = pos.y; j <= pos.y + dim.h; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			if (is_animated(t))
			{
				animation[t].draw(i * tilesize, j * tilesize, cam, renderer);
			}
			
			src = source[t];
		
			if (i == pos.x)
			{
				src.x += mod_x;
				src.w = tilesize - mod_x;
				dest.x = dim.x;
			}
			else if (i == pos.x + dim.w)
			{
				if (mod_x == 0)
					continue;
				
				src.w = mod_x;
				dest.x = dim.x + dim.w * tilesize - src.w;
			}
			else
			{
				dest.x = dim.x + (i - pos.x) * tilesize - (mod_x);
			}
			
			if (j == pos.y)
			{
				src.y += mod_y;
				src.h = tilesize - mod_y;
				dest.y = dim.y;
			}
			else if (j == pos.y + dim.h)
			{
				if (mod_y == 0)
					continue;
				
				src.h = mod_y;
				dest.y = dim.y + dim.h * tilesize - src.h;
			}
			else
			{
				dest.y = dim.y + (j - pos.y) * tilesize - (mod_y);
			}

			if (texture)
			{

				dest.x += x;
				dest.y += y;
				dest.w = src.w;
				dest.h = src.h;
				SDL_RenderCopy(renderer, texture, &src, &dest);
			}
		}
}

void CTileMapView::draw ( int x, int y, SDL_Renderer * renderer )
{
	int i, j, t;
	SVect pos, p;
	SDL_Rect dest ={0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, width, height};


	dim.w /= tilesize;
	dim.h /= tilesize;
	
	for (i = 0; i <= dim.w; i++)
		for (j = 0; j <= dim.h; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			if (is_animated(t))
			{
				animation[t].draw(i * tilesize, j * tilesize, renderer);
			}

			src = source[t];

			if (texture)
			{
				dest.x += x;
				dest.y += y;
				dest.w = tilesize;
				dest.h = tilesize;
				SDL_RenderCopy(renderer, texture, &src, &dest);
			}
		}
}


