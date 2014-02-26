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

#endif

