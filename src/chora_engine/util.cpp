#include "util.hpp"

Uint32 get_pixel ( SDL_Surface *surface, int x, int y )
{
	Uint32 * p = (Uint32 *)surface->pixels;

	return p[(y * surface->w) + x];
	/*
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to retrieve
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32 *)p;
			break;

		default:
			return 0;       // shouldn't happen, but avoids warnings
	}
	*/
}

void put_pixel ( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	Uint32 * p = (Uint32 *)surface->pixels;

	p[(y * surface->w) + x] = pixel;
	/*
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to set
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

		switch(bpp) {
			case 1:
				*p = pixel;
				break;

			case 2:
				*(Uint16 *)p = pixel;
				break;

			case 3:
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
				} else {
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
				}
				break;

			case 4:
				*(Uint32 *)p = pixel;
				break;

			default:
				break;
	}
	*/
}


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
		if (cam)
		{
			r.x = (r.x - cam->get_position().x) + cam->get_dimension().x;
			r.y = (r.y - cam->get_position().y) + cam->get_dimension().y;
		}
	
		SDL_FillRect(screen, &r, color);
	}

	void draw_surface ( SDL_Surface * surface, int x, int y, CCamera * cam, SDL_Surface * screen )
	{
		if (!surface)
			return;
		
		SDL_Rect dest, source = {0,0,surface->w,surface->h};
		
		if (source.w == 0 && source.h == 0)
			return;
		
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
				//source.w = 0; // não pode ser 0 no emscripten
				return;

			dest.x = dim.x;
		}
		else if (dest.x + dest.w > pos.x + dim.x + dim.w)
		{
			if (dest.x + dest.w - (pos.x + dim.x + dim.w) < source.w)
				source.w -= dest.x + dest.w - (pos.x + dim.x + dim.w);
			else
				//source.w = 0; // não pode ser 0 no emscripten
				return;
		
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
				//source.h = 0; // não pode ser 0 no emscripten
				return;

			dest.y = dim.y;
		}
		else if (dest.y + dest.h > pos.y + dim.y + dim.h)
		{
			if (dest.y + dest.h - (pos.y + dim.y + dim.h) < source.h)
				source.h -= dest.y + dest.h - (pos.y + dim.y + dim.h);
			else
				//source.h = 0; // não pode ser 0 no emscripten
				return;
		
			dest.y = dest.y - pos.y;
		}
		else
		{
			dest.y = dest.y - pos.y;
		}
	
		dest.w = source.w;
		dest.h = source.h;
	
		SDL_BlitSurface(surface, &source, screen, &dest);
	}

	SDL_Surface * clone_surface ( SDL_Surface * surf, SDL_Rect src )
	{
		if (!surf || src.x < 0 || src.y < 0 || src.x + src.w > surf->w || src.y + src.h > surf->h)
			return 0;

		Uint32 rmask, gmask, bmask, amask;

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		#endif

		#ifndef __clang__
		Uint32 colorkey = 0;
		int setkey = 0;
		if(surf->flags & SDL_SRCCOLORKEY)
		{
			setkey = 1;
			colorkey = surf->format->colorkey;
			SDL_SetColorKey(surf, 0, 0);
		}
		#endif

		SDL_Surface * tmp;
		if (surf->format->Amask)
			tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, src.w, src.h, surf->format->BitsPerPixel,
			                                          rmask, gmask, bmask, amask);
		else
			tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, src.w, src.h, surf->format->BitsPerPixel,
		                                            surf->format->Rmask, surf->format->Gmask, surf->format->Bmask, surf->format->Amask);

		if (!tmp)
			throw "clone_surface: não foi possivel criar surface\n";
/*
		if (SDL_MUSTLOCK(surf))
			SDL_LockSurface(surf);
		if (SDL_MUSTLOCK(tmp))
			SDL_LockSurface(tmp);

		for (int i = 0; i < surf->h; i++)
			for (int j = 0; j < surf->h; j++)
			{
				SDL_Rect p = {0,0,1,1};
				SDL_FillRect(tmp, &p, get_pixel(surf, j, i));
			}

		if (SDL_MUSTLOCK(surf))
			SDL_UnlockSurface(surf);
		if (SDL_MUSTLOCK(tmp))
			SDL_UnlockSurface(tmp);		
*/
		if (SDL_BlitSurface(surf, &src, tmp, NULL) < 0)
			throw "clone_surface: blit falhou\n";

		#ifndef __clang__
		if (setkey)
		{
			SDL_SetColorKey(surf, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
			SDL_SetColorKey(tmp, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
		}
		#endif

		return tmp;
	}

	SDL_Surface * mirror_surface ( SDL_Surface * surf, int flip )
	{
		if (!surf)
			return 0;

		Uint32 rmask, gmask, bmask, amask;

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		#endif

		SDL_Surface * tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, surf->w, surf->h, surf->format->BitsPerPixel,
		                                                 surf->format->Rmask, surf->format->Gmask, surf->format->Bmask, surf->format->Amask);

		if (!tmp)
			throw "mirror_surface: não foi possível criar surface\n";

/*
		if(SDL_MUSTLOCK(surf))
			SDL_LockSurface(surf);
		if(SDL_MUSTLOCK(tmp))
			SDL_LockSurface(tmp);
*/
		#ifndef __clang__
		Uint32 colorkey = 0;
		int setkey = 0;
		if (surf->flags & SDL_SRCCOLORKEY)
		{
			setkey = 1;
			colorkey = surf->format->colorkey;
			SDL_SetColorKey(surf, 0, 0);
		}
		#endif
		
		if (flip == FLIP_HOR)
		{
			SDL_Rect d, s;
			for (int i = 0; i < surf->w; i++)
			{
				d.y = s.y = 0;
				d.h = s.h = surf->h;

				s.x = i;
				d.x = (surf->w - 1) - i;
				d.w = s.w = 1;
				if (SDL_BlitSurface(surf, &s, tmp, &d) < 0)
					throw "mirror_surface: blit falhou\n";
			}
		}
		else
		{
			SDL_Rect d, s;
			for (int i = 0; i < surf->h; i++)
			{
				s.y = i;
				d.y = (surf->h - 1) - i;
				d.h = s.h = 1;

				d.x = s.x = 0;
				d.w = s.w = surf->w;
				if (SDL_BlitSurface(surf, &s, tmp, &d) < 0)
					throw "mirror_surface: blit falhou\n";
			}
		}
/*
		if (SDL_MUSTLOCK(tmp))
			SDL_UnlockSurface(tmp);
		if (SDL_MUSTLOCK(surf))
			SDL_UnlockSurface(surf);
*/
		
		#ifndef __clang__
		if(setkey)
		{
			SDL_SetColorKey(surf, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
			SDL_SetColorKey(tmp, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
		}
		#endif

		return tmp;
	}

#else
	SDL_Surface * optimize_surface ( SDL_Surface * s, SDL_Surface * screen )
	{
		if (!s || !screen)
			return 0;

		return SDL_ConvertSurface(s, screen->format, 0);
	}

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
		
		if (source.w == 0 && source.h == 0)
			return;
			
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
				//source.w = 0; // não pode ser zero no emscripten
				return;

			dest.x = dim.x;
		}
		else if (dest.x + dest.w > pos.x + dim.x + dim.w)
		{
			if (dest.x + dest.w - (pos.x + dim.x + dim.w) < source.w)
				source.w -= dest.x + dest.w - (pos.x + dim.x + dim.w);
			else
				//source.w = 0; // não pode ser zero no emscripten
				return;
		
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
				//source.h = 0; // não pode ser zero no emscripten
				return;

			dest.y = dim.y;
		}
		else if (dest.y + dest.h > pos.y + dim.y + dim.h)
		{
			if (dest.y + dest.h - (pos.y + dim.y + dim.h) < source.h)
				source.h -= dest.y + dest.h - (pos.y + dim.y + dim.h);
			else
				//source.h = 0; // não pode ser zero no emscripten
				return;
		
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


