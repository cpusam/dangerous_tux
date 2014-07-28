#include "animation.hpp"

void CAnimationFrame::set_anim ( int d, SDL_Rect src )
{
	set_source(src);
	set_delay(d);
}

bool CAnimationFrame::set_delay ( int d )
{
	if (d > -1)
		delay = d;
	
	return (d > -1);
}

int CAnimationFrame::get_delay (  )
{
	return delay;
}

void CAnimationFrame::set_source ( SDL_Rect s )
{
	source = s;
}

SDL_Rect CAnimationFrame::get_source (  )
{
	return source;
}

//////////////////////////////////////////////////////////////////

void CAnimation::play (  )
{
	if (get_state() == STOPED_ANIM)
		set_state(START_ANIM);
	else
		set_state(RUNNING_ANIM);
	
	timer.start();
}

void CAnimation::pause (  )
{
	set_state(PAUSE_ANIM);
	timer.pause();
}

void CAnimation::reset (  )
{
	index = 0;
	set_state(START_ANIM);
	timer.reset();
}

void CAnimation::set_repeat ( bool r )
{
	repeat = r;
}

STimer CAnimation::get_timer (  )
{
	return timer;
}

void CAnimation::set_delay ( int f, int d )
{
	if (f >= 0 && f < frames.size())
		frames[f].set_delay(d);
}

// seta todos os frames para o mesmo delay
void CAnimation::set_frames_delay ( int d )
{
	for (int i = 0; i < frames.size(); i++)
		frames[i].set_delay(d);
}

int CAnimation::get_frames_size (  )
{
	return frames.size();
}

void CAnimation::clear_frames ( bool destroy )
{
	frames.clear();
	#ifndef USE_SDL2
		if (destroy)
			destroy_surfaces();
		surface.clear();
	#else
		if (destroy)
			destroy_textures();
		texture.clear();
	#endif
}

#ifndef USE_SDL2
	void CAnimation::add_frame ( SDL_Surface * s, SDL_Rect src, int d )
	{
		index = 0;
		surface.push_back(s);
		frames.push_back(CAnimationFrame(d, src));
	}
	
	void CAnimation::add_frame ( SDL_Surface * s, CAnimationFrame f )
	{
		index = 0;
		surface.push_back(s);
		frames.push_back(f);
	}
	
	void CAnimation::destroy_surfaces (  )
	{
		std::vector <SDL_Surface *> s;
		
		for (int i(0); i < surface.size(); i++)
		{
			SDL_Surface * aux = surface[i];
			int count = 0;
			for (int j(0); j < surface.size(); j++)
				if (j != i && aux == surface[j])
					count++;
			
			if (count == 0)
			{
				if (aux)
					s.push_back(aux);
			}
			else
			{
				count = 0;
				for (int j(0); j < s.size(); j++)
					if (s[j] == aux)
						count++;
				
				if (count > 0)
					if (aux)
						s.push_back(aux);
			}
		}
		
		for (int i(0); i < s.size(); i++)
			if (s[i])
				SDL_FreeSurface(s[i]);
		
		surface.clear();
		s.clear();
	}
	
	SDL_Surface * CAnimation::get_surface ( int i )
	{
		if (surface.size() > 0 && i < surface.size())
			return surface[i];
		
		return NULL;
	}
	
	bool CAnimation::has_surface ( SDL_Surface * s )
	{
		if (!s)
			return false;
		
		for (int i(0); i < surface.size(); i++)
			if (surface[i] == s)
				return true;
		
		return false;
	}
#else
	void CAnimation::add_frame ( SDL_Texture * t, SDL_Rect src, int d )
	{
		index = 0;
		texture.push_back(t);
		frames.push_back(CAnimationFrame(d, src));
	}
	
	void CAnimation::add_frame ( SDL_Texture * t, CAnimationFrame f )
	{
		index = 0;
		texture.push_back(t);
		frames.push_back(f);
	}
	
	SDL_Texture * CAnimation::get_texture ( int i )
	{
		if (texture.size() > 0 && i < texture.size())
			return texture[i];
		
		return NULL;
	}
	
	void CAnimation::destroy_textures (  )
	{
		std::vector <SDL_Texture *> t;
		
		for (int i(0); i < texture.size(); i++)
		{
			SDL_Texture * aux = texture[i];
			int count = 0;
			for (int j(0); j < texture.size(); j++)
				if (j != i && aux == texture[j])
					count++;
			
			if (count == 0)
			{
				if (aux)
					t.push_back(aux);
			}
			else
			{
				count = 0;
				for (int j(0); j < t.size(); j++)
					if (t.at(j) == aux)
						count++;
				
				if (count > 0)
					if (aux)
						t.push_back(aux);
			}
		}
		
		for (int i(0); i < t.size(); i++)
			if (t.at(i))
				SDL_DestroyTexture(t.at(i));
		
		texture.clear();
		t.clear();
	}
	
	bool CAnimation::has_texture ( SDL_Texture * t )
	{
		if (!t)
			return false;
		
		for (int i(0); i < texture.size(); i++)
			if (t == texture.at(i))
				return true;
		
		return false;
	}
#endif

bool CAnimation::set_index ( int i )
{
	if (i >= 0 && i < frames.size())
	{
		index = i;
		timer.reset();
		return true;
	}
	
	return false;
}

int CAnimation::get_index (  )
{
	return index;
}

CAnimationFrame CAnimation::get_frame ( int i )
{
	if (i > 0 && i <= frames.size())
		return frames[i];
	
	return CAnimationFrame();
}

CAnimationFrame CAnimation::get_curr_frame (  )
{
	return frames[index];
}

#ifndef USE_SDL2
	void CAnimation::draw ( int x, int y, SDL_Surface * screen )
#else
	void CAnimation::draw ( int x, int y, SDL_Renderer * renderer )
#endif
{
	SDL_Rect dest, source;
	dest.x = x + frames[index].x;
	dest.y = y + frames[index].y;
	source = frames[index].get_source();
	
	dest.w = source.w;
	dest.h = source.h;
	
	#ifndef USE_SDL2
		if (surface.size() && surface.at(index))
			SDL_BlitSurface(surface.at(index), &source, screen, &dest);
	#else
		if (texture.size() && texture.at(index))
			SDL_RenderCopy(renderer, texture.at(index), &source, &dest);
	#endif
}

#ifndef USE_SDL2
	void CAnimation::draw ( int x, int y, CCamera * cam, SDL_Surface * screen )
#else
	void CAnimation::draw ( int x, int y, CCamera * cam, SDL_Renderer * renderer )
#endif
{
	SDL_Rect dest, source;
	source = frames[index].get_source();
	
	SVect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();

	dest.x = x + dim.x + frames[index].x;
	dest.y = y + dim.y + frames[index].y;
	dest.w = source.w;
	dest.h = source.h;

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
	
	#ifndef USE_SDL2
		if (surface.size() && surface.at(index))
			SDL_BlitSurface(surface.at(index), &source, screen, &dest);
	#else
		if (texture.size() && texture.at(index))
			SDL_RenderCopy(renderer, texture.at(index), &source, &dest);
	#endif
}

int CAnimation::update (  )
{
	switch (get_state())
	{
		case START_ANIM:
		case CHANGE_FRAME_ANIM:
		case RUNNING_ANIM:
		case FINISHED_ANIM:
			if (frames.size() == 0)
				throw "CAnimation: animação sem frames\n";

			timer.update();
			if (timer.steps() >= frames[index].get_delay())
			{
				timer.reset();
				index++;
				if (index >= frames.size())
				{
					if (repeat)
					{
						index = 0;
						set_state(FINISHED_ANIM); // termina e repete a animação
						break;
					}
					else
					{
						index = int(frames.size() - 1);
						set_state(STOPED_ANIM); // terminou a animação e fica parado
						break;
					}
				}
				
				set_state(CHANGE_FRAME_ANIM); // novo frame
				break;
			}

			set_state(RUNNING_ANIM); // rodando
			break;

		default:
			break;
	}
	
	return get_state();
}


