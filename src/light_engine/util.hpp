/*
	funções de uso geral
*/

#ifndef LIGHTUTIL_HPP
#define LIGHTUTIL_HPP

SDL_Surface * optimize_surface ( SDL_Surface * s )
{
	SDL_Surface * tmp = 0;
	
	if (!s)
		return 0;
	
	tmp = SDL_DisplayFormat(s);
	
	if (tmp)
		SDL_FreeSurface(s);
	
	return tmp;
}

SDL_Surface * optimize_surface_alpha ( SDL_Surface * s )
{
	SDL_Surface * tmp = 0;
	
	if (!s)
		return 0;
	
	tmp = SDL_DisplayFormatAlpha(s);
	
	if (tmp)
		SDL_FreeSurface(s);
	
	return tmp;
}

void fill_rect ( CCamera * cam, SDL_Surface * screen, Uint32 color, SDL_Rect r )
{
	SDL_Rect d;

	if (cam)
	{
		d.x = (r.x - cam->get_position().x) + cam->get_dimension().x;
		d.y = (r.y - cam->get_position().y) + cam->get_dimension().y;
	}
	else
	{
		d.x = r.x;
		d.y = r.y;
	}
	
	d.w = r.w;
	d.h = r.h;
	
	SDL_FillRect(screen, &d, color);
}

void draw_surface ( SDL_Surface * surface, int x, int y, CCamera * cam, SDL_Surface * screen )
{
	SDL_Rect dest, source = {0,0,surface->w,surface->h};
	
	SVect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();

	dest.x = x + dim.x;
	dest.y = y + dim.y;

	if (dest.x < dim.x + pos.x)
	{
		source.x += (dim.x + pos.x) - dest.x;
		if (((dim.x + pos.x) - dest.x) < source.w)
			source.w -= ((dim.x + pos.x) - dest.x);
		else
			source.w = 0;

		dest.x = dim.x;
	}
	else if (dest.x + dest.w > pos.x + dim.x + dim.w)
	{
		if (dest.x + dest.w - (pos.x + dim.x + dim.w) < source.w)
			source.w -= dest.x + dest.w - (pos.x + dim.x + dim.w);
		else
			source.w = 0;
		
		dest.x = dest.x - pos.x;
	}
	else
	{
		dest.x = dest.x - pos.x;
	}
	
	if (dest.y < dim.y + pos.y)
	{
		source.y += (dim.y + pos.y) - dest.y;
		if (((dim.y + pos.y) - dest.y) < source.h)
			source.h -= ((dim.y + pos.y) - dest.y);
		else
			source.h = 0;

		dest.y = dim.y;
	}
	else if (dest.y + dest.h > pos.y + dim.y + dim.h)
	{
		if (dest.y + dest.h - (pos.y + dim.y + dim.h) < source.h)
			source.h -= dest.y + dest.h - (pos.y + dim.y + dim.h);
		else
			source.h = 0;
		
		dest.y = dest.y - pos.y;
	}
	else
	{
		dest.y = dest.y - pos.y;
	}
	
	dest.w = source.w;
	dest.h = source.h;
	
	if (surface)
		SDL_BlitSurface(surface, &source, screen, &dest);
}

#endif

