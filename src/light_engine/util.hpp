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

/*
	funções de uso geral
*/
#ifndef LIGHTUTIL_HPP
#define LIGHTUTIL_HPP

#ifndef USE_SDL2
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

#else
	int texture_width ( SDL_Texture * t )
	{
		int w;
		if (!t)
			return 0;
		
		SDL_QueryTexture(t, NULL, NULL, &w, NULL);
		return w;
	}
	
	int texture_height ( SDL_Texture * t )
	{
		int h;
		if (!t)
			return 0;
		
		SDL_QueryTexture(t, NULL, NULL, NULL, &h);
		return h;
	}

	void fill_rect ( CCamera * cam, SDL_Renderer * renderer, Uint32 color, SDL_Rect r )
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
		
		SDL_SetRenderDrawColor(renderer, (color & 0xFF000000) >> 24, (color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, (color & 0x000000FF));
		SDL_RenderFillRect(renderer, &d);
	}

	void draw_texture ( SDL_Texture * texture, int x, int y, CCamera * cam, SDL_Renderer * renderer )
	{
		if (!texture)
			return;

		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_Rect dest, source = {0,0,w,h};
	
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
		
		SDL_RenderCopy(renderer, texture, &source, &dest);
	}
#endif

#endif

