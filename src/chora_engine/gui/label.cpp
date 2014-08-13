#include "label.hpp"

void CLabel::str_to_surface ( std::string s )
{
	int w, h;
	#ifndef USE_SDL2
		if (surface)
			SDL_FreeSurface(surface);

		surface = CWriter::instance()->render_text(s, color, UTF8_TEXT);
		if (!surface)
			throw SDL_GetError();
		
		w = surface->w;
		h = surface->h;
	#else
		if (texture)
			SDL_DestroyTexture(texture);
		
		if (s != "")
			texture = CWriter::instance()->render_text(s, color, UTF8_TEXT);
		else
		{
			texture = 0;
			return;
		}
		
		if (!texture)
		{
			char * e = new char[256];
			sprintf(e, "CLabel: erro %s\n", SDL_GetError());
			throw (char *)e; // c++ esquisito
		}
		
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	#endif
	
	dim.x = pos.x + rel_pos.x, dim.y = pos.y + rel_pos.y;
	dim.w = w, dim.h = h;
}

void CLabel::set_color ( SDL_Color c )
{
	color = c;
	str_to_surface(str);
}

void CLabel::set_str ( std::string s )
{
	if (s == str)
		return;

	str = s;

	str_to_surface(s);
}

#ifndef USE_SDL2
	SDL_Surface * CLabel::get_surface (  )
	{
		return surface;
	}

	void CLabel::set_surface ( SDL_Surface * s )
	{
		if (surface)
			SDL_FreeSurface(surface);

		surface = s;
		dim.w = surface->w;
		dim.h = surface->h;
	}
#else
	SDL_Texture * CLabel::get_texture (  )
	{
		return texture;
	}
	
	void CLabel::set_texture ( SDL_Texture * t )
	{
		if (texture && texture != t)
			SDL_DestroyTexture(texture);
		
		texture = t;
		
		if (texture)
		{
			int w, h;
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			dim.w = w;
			dim.h = h;
		}
		else
		{
			dim.w = dim.h = 0;
		}
	}
	
	int CLabel::get_texture_width (  )
	{
		int w;
		
		if (!texture)
			return 0;
			
		SDL_QueryTexture(texture, NULL, NULL, &w, NULL);
		return w;
	}
	
	int CLabel::get_texture_height (  )
	{
		int h;
		
		if (!texture)
			return 0;
		
		SDL_QueryTexture(texture, NULL, NULL, NULL, &h);
		return h;
	}
#endif

std::string CLabel::get_str (  )
{
	return str;
}

#ifndef USE_SDL2
	void CLabel::draw ( SDL_Surface * screen )
#else
	void CLabel::draw ( SDL_Renderer * renderer )
#endif
{
	if (!visible)
		return;

	SDL_Rect d = dim;
	#ifndef USE_SDL2
		if (surface)
			SDL_BlitSurface(surface, NULL, screen, &d);
		child_draw(screen);
	#else
		if (texture)
			SDL_RenderCopy(renderer, texture, NULL, &d);
		child_draw(renderer);
	#endif
}

///////////////////////////////////////////////////////////

void CLabelNumber::proc_value ( float v ) // para ser usada internamente
{
	int n = v, a, i;
	std::string s, aux;
	value = v;

	for (i = 0; n > 0; i++)
	{
		a = n % 10;
		n = (n - a) / 10;
		aux.push_back('0' + a);
	}

	// inverte aux;
	for (i = aux.size() - 1; i > -1; i--)
		s.push_back(aux[i]);

	aux.swap(s);
	s.clear();

	if (aux.size() < left_zero)
	{
		for (i = 0; i < left_zero - aux.size(); i++)
			s.push_back('0');

		s.append(aux);
	}
	else
	{
		s.swap(aux);
	}

	set_str(s);
}

void CLabelNumber::set_left_zero ( int lz )
{
	if (lz > -1 && lz != left_zero)
	{
		left_zero = lz;			
		proc_value(value);
	}
}

bool CLabelNumber::set_value ( float v )
{
	if (value == v)
		return false;

	proc_value(int(v));
	value = v;

	return true;
}

/////////////////////////////////////////////////////////////

void CTextInput::set_cursor_size ( int w, int h )
{
	cursor.w = w;
	cursor.h = h;
}

void CTextInput::set_pos ( SVect p )
{
	CWidget::set_pos(p);
	#ifndef USE_SDL2
		cursor.x = pos.x + rel_pos.x + get_surface()->w;
	#else
		cursor.x = pos.x + rel_pos.x + get_texture_width();
	#endif
	cursor.y = pos.y + rel_pos.y;
}

void CTextInput::set_rel_pos ( SVect p )
{
	CWidget::set_rel_pos(p);
	#ifndef USE_SDL2
		cursor.x = pos.x + rel_pos.x + get_surface()->w;
	#else
		cursor.x = pos.x + rel_pos.x + get_texture_width();
	#endif
	cursor.y = pos.y + rel_pos.y;
}

void CTextInput::input ( SDL_Event & event )
{
	if (!visible)
		return;
		
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_BACKSPACE)
		{
			if (str.size() > 0)
			{
				str.erase(str.end() - 1);
				str_to_surface(str);
				#ifndef USE_SDL2
					cursor.x = pos.x + rel_pos.x + get_surface()->w;
				#else
					cursor.x = pos.x + rel_pos.x + get_texture_width();
				#endif
			}
		}
		else if (event.key.keysym.sym == SDLK_CAPSLOCK)
		{
			if (caps_lock)
				caps_lock = false;
			else
				caps_lock = true;
		}				
		else if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
		{
			shift_key = true;
		}
		else// if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z)
		{
			if (str.size() < strsize)
			{
				std::stringstream s;
				s << str;

				if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)
				{
					if (caps_lock || shift_key)
						s << char(toupper(char(event.key.keysym.sym)));
					else
						s << char(event.key.keysym.sym);
				}
				else
					s << char(event.key.keysym.sym);
			
				str = s.str();
				str_to_surface(str);
				#ifndef USE_SDL2
					cursor.x = pos.x + rel_pos.x + get_surface()->w;
				#else
					int w;
					SDL_QueryTexture(get_texture(), NULL, NULL, &w, NULL);
					cursor.x = pos.x + rel_pos.x + w;
				#endif
			}
		}
	}
	
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
			shift_key = false;
	}
	
	child_input(event);
}

#ifndef USE_SDL2
	void CTextInput::draw ( SDL_Surface * screen )
	{
		if (!visible)
			return;

		SDL_Rect d;

		if ((++count) % 10 < 5)
		{
			d = cursor;
			SDL_FillRect(screen, &d, cursor_color);
		}
	
		CLabel::draw(screen);
	}
#else
	void CTextInput::draw ( SDL_Renderer * renderer )
	{
		if (!visible)
			return;

		SDL_Rect d;

		if ((++count) % 10 < 5)
		{
			d = cursor;
			SDL_SetRenderDrawColor(renderer, (cursor_color & 0xFF000000) >> 24, (cursor_color & 0x00FF0000) >> 16, (cursor_color & 0x0000FF00) >> 8, (cursor_color & 0x000000FF));
			SDL_RenderFillRect(renderer, &d);
		}
	
		CLabel::draw(renderer);
	}
#endif

