#include "tilemapview.hpp"

#include <algorithm>

////////////////////////////////////////////////////////////////////
CAnimatedTile TileMapView::get_animation ( int tile )
{
	for (unsigned int i = 0; i < animation.size(); i++)
		if (animation.at(i).get_tile() == tile)
			return animation.at(i);
	
	return CAnimatedTile();
}

SDL_Rect TileMapView::get_source ( int tile )
{
	if (source.count(tile) > 0)
		return source[tile];
	
	return (SDL_Rect){0,0,-1,-1};
}

void TileMapView::add_animation ( CAnimatedTile & a, int t )
{
	//if (!texture)
		//throw "TileMapView: texture nula";
	//a.set_texture(texture);

	a.set_tile(t);
	add_tile(t);
	#ifdef OLD_ANIMATED
	animation[t] = a;
	#else
	if (!is_animated(t))
	{
		animatedTilesID.push_back(t);
		animatedTiles.push_back(a);
	}
	#endif
}

bool TileMapView::is_animated (	int t )
{
	#ifdef OLD_ANIMATED
	return (animation.find(t) != animation.end());
	#else
	return (std::find(animatedTilesID.begin(), animatedTilesID.end(), t) != animatedTilesID.end());
	#endif
}

void TileMapView::update_animation (  )
{
	#ifdef OLD_ANIMATED
	for (std::vector <int>::iterator i = tiles.begin(); i != tiles.end(); i++)
	{
		if (is_animated(*i))
			animation[*i].update();
	}
	#else
	for (auto & it: animatedTiles)
		it.update();
	#endif
}



int TileMapView::draw ( SDL_Renderer * renderer, Camera * cam )
{
	if (!texture)
		return -1;

	int i, j, t, ret = 0;
	Vect pos = cam->get_position() - this->pos, pCam = cam->get_position() - this->pos;
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = cam->get_dimension();

	Vect p;
	p.x = int(pos.x) / tilesize;
	p.y = int(pos.y) / tilesize;
	dim.x = 0;
	dim.y = 0;
	dim.w /= tilesize;
	dim.h /= tilesize;
	int mod_x = int(pCam.x) % tilesize;
	int mod_y = int(pCam.y) % tilesize;
	int maxHor = p.x + dim.w + 1;
	int maxVer = p.y + dim.h + 1;

	//para o caso do mapa ser menor que as dimensões da camera
	if (width < dim.w + 1)
	{
		mod_x = 0;
		maxHor -= 1;
	}

	if (height < dim.h + 1)
	{
		mod_y = 0;
		maxVer -= 1;
	}
	
	if (!tileset.size())
	{
		std::cout<<"TileMapView::Error não tem tiles\n";
		return -1;
	}

	for (i = p.x - 1; i <= maxHor; i++)
		for (j = p.y - 1; j <= maxVer; j++)
		{
			int index = j * width + i;
			if (index < 0 || index >= int(tileset.size()))
				continue;
			
			t = tileset[index];
			//t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			dest.x = ((i - p.x) * tilesize + dim.x) - mod_x;
			dest.y = ((j - p.y) * tilesize + dim.y) - mod_y; 

			dest.w = tilesize;
			dest.h = tilesize;

			if (is_animated(t))
			{
				#ifdef OLD_ANIMATED
					animation[t].draw(renderer, dest.x, dest.y);
				#else
				for (int k = 0; k < animatedTilesID.size(); k++)
					if (animatedTilesID[k] == t)
					{
						AnimationFrame frame = animatedTiles[k].get_curr_frame();
						src = frame.get_source();
						SDL_Point center = {src.w/2, src.h/2};
						SDL_Texture * textureAnim = animatedTiles[k].get_texture();
						ret = SDL_RenderCopyEx(renderer, textureAnim, &src, &dest, animatedTiles[k].get_angle(), &center, frame.get_flip());
						break;
					}
				#endif
				continue;
			}

			src = source[t];
			ret = SDL_RenderCopy(renderer, texture, &src, &dest);
		
			if (ret < 0)
				break;
		}
	
	return ret;
}

int TileMapView::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y )
{
	if (!texture)
		return -1;
	
	int i, j, t, ret = 0;
	Vect pos = cam->get_position() - this->pos;
	Vect p = cam->get_position() - this->pos;
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, cam->get_dimension().w, cam->get_dimension().h};


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



	for (i = pos.x - 1; i <= pos.x + dim.w + 1; i++)
		for (j = pos.y - 1; j <= pos.y + dim.h + 1; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			//é preciso parenteses extras para evitar bugs
			dest.x = ((i - pos.x) * tilesize + dim.x + x) - mod_x;
			dest.y = ((j - pos.y) * tilesize + dim.y + y) - mod_y;

			dest.w = tilesize;
			dest.h = tilesize;

			if (is_animated(t))
			{
				#ifdef OLD_ANIMATED
					animation[t].draw(renderer, dest.x, dest.y);
				#else
				for (int k = 0; k < animatedTilesID.size(); k++)
					if (animatedTilesID[k] == t)
					{
						AnimationFrame frame = animatedTiles[k].get_curr_frame();
						src = frame.get_source();
						SDL_Point center = {src.w/2, src.h/2};
						SDL_Texture * textureAnim = animatedTiles[k].get_texture();
						ret = SDL_RenderCopyEx(renderer, textureAnim, &src, &dest, animatedTiles[k].get_angle(), &center, frame.get_flip());
						break;
					}
				#endif
				continue;
			}

			{
				src = source[t];
				ret = SDL_RenderCopy(renderer, texture, &src, &dest);
			}
			
			if (ret < 0)
				break;
			
				break;

		}
	
	return ret;
}

int TileMapView::draw ( SDL_Renderer * renderer, int x, int y )
{
	if (!texture)
		return -1;
	
	int i, j, t, ret = 0;
	Vect pos, p;
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, width, height};


	dim.w /= tilesize;
	dim.h /= tilesize;

	for (i = -1; i <= dim.w+1; i++)
		for (j = -1; j <= dim.h+1; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;
			
			dest.x += x;
			dest.y += y;

			dest.w = tilesize;
			dest.h = tilesize;
			
			if (is_animated(t))
			{
				#ifdef OLD_ANIMATED
					animation[t].draw(renderer, dest.x, dest.y);
				#else
				for (int k = 0; k < animatedTilesID.size(); k++)
					if (animatedTilesID[k] == t)
					{
						AnimationFrame frame = animatedTiles[k].get_curr_frame();
						src = frame.get_source();
						SDL_Point center = {src.w/2, src.h/2};
						SDL_Texture * textureAnim = animatedTiles[k].get_texture();
						ret = SDL_RenderCopyEx(renderer, textureAnim, &src, &dest, animatedTiles[k].get_angle(), &center, frame.get_flip());
						break;
					}
				#endif
				continue;
			}

			//if (texture)
			{
				src = source[t];
				ret = SDL_RenderCopy(renderer, texture, &src, &dest);
			}
			
			if (ret < 0)
				break;
		}
	
	return ret;
}


