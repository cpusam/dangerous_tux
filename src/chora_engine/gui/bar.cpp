#include "bar.hpp"

void CBar::set_orientation ( int o )
{
	ori = o;
}

void CBar::set_direction ( int d )
{
	dir = d;
}

void CBar::set_size ( float s )
{
	size = s;
	
	if (size < 0)
		size = 0;
	else if (size > full_size)
		size = full_size;
}

void CBar::reset (  )
{
	size = full_size;
}

void CBar::add ( float s )
{
	size += s;
	if (size < 0)
		size = 0;
	else if (size > full_size)
		size = full_size;
}

#ifndef USE_SDL2
	void CBar::draw ( SDL_Surface * screen )
#else
	void CBar::draw ( SDL_Renderer * renderer )
#endif
{
	if (!visible)
		return;

	SDL_Rect d;

	d.x = pos.x;
	d.y = pos.y;
	d.w = dim.w;
	d.h = dim.h;
	
	#ifndef USE_SDL2
		SDL_FillRect(screen, &d, color_bg);
	#else
		SDL_SetRenderDrawColor(renderer, (color_bg & 0xFF000000) >> 24, (color_bg & 0x00FF0000) >> 16, (color_bg & 0x0000FF00) >> 8, (color_bg & 0x000000FF));
		
		SDL_RenderFillRect(renderer, &d);
	#endif
	
	if (ori == 0)
	{
		if (dir == 0) // da direita para esquerda
		{
			d.x = pos.x;
			d.w = int(size/full_size * dim.w);
			d.y = pos.y;
			d.h = dim.h;
		}
		else // da esquerda para direita
		{
			d.x = pos.x + (dim.w - int(size/full_size * dim.w));
			d.w = dim.w;
			d.y = pos.y;
			d.h = dim.h;
		}
	}
	else
	{
		if (dir == 0) // baixo para cima
		{
			d.x = pos.x;
			d.w = dim.w;
			d.y = pos.y;
			d.h = int(size/full_size * dim.h);
		}
		else // cima para baixo
		{
			d.x = pos.x;
			d.w = dim.w;
			d.y = pos.y + (dim.h - int(size/full_size * dim.h));
			d.h = int(size/full_size * dim.h);
		}
	}
	
	#ifndef USE_SDL2
		SDL_FillRect(screen, &d, color_bar);
	#else
		SDL_SetRenderDrawColor(renderer, (color_bar & 0xFF000000) >> 24, (color_bar & 0x00FF0000) >> 16, (color_bar & 0x0000FF00) >> 8, (color_bar & 0x000000FF));
		
		SDL_RenderFillRect(renderer, &d);
	#endif
}


