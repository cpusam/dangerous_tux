#ifndef ANIMATION_HPP
#define ANIMATION_HPP

struct STimer
{
	int state;
	float time;
	float step;
	
	STimer (  )
	{
		time = 0;
		step = 1;
	}
	
	void start (  )
	{
		state = 1;
	}
	void pause (  )
	{
		state = 0;
	}
	
	void reset (  )
	{
		time = 0;
	}
	
	int steps (  )
	{
		return int(time/step);
	}
	
	void update (  )
	{
		if (state)
			time += step;
	}
};


class CAnimation: protected CStateMachine
{
	protected:
		STimer timer;
		int state;
		int index;
		bool repeat;
		vector <int> delay;
		vector <SDL_Rect> frames;

		#ifndef USE_SDL2
			public:
				SDL_Surface * surface;
		#else
			protected:
				vector <SDL_Texture *> texture;
		#endif

	public:
		CAnimation (  )
		{
			set_state(1);
			repeat = true;
			index = 0;
			timer.start();
			#ifndef USE_SDL2
				surface = 0;
			#endif
		}
		
		void play (  )
		{
			set_state(1);
			timer.start();
		}
		
		void pause (  )
		{
			set_state(0);
			timer.pause();
		}
		
		void reset (  )
		{
			index = 0;
			timer.reset();
		}
		
		void set_repeat ( bool r )
		{
			repeat = r;
		}
		
		STimer get_timer (  )
		{
			return timer;
		}
		
		void set_delay ( int f, int d )
		{
			if (f < delay.size())
				delay[f] = d;
			else
				delay.push_back(d);
		}
		
		// seta todos os frames para o mesmo delay
		void set_frames_delay ( int d )
		{
			for (int i = 0; i < delay.size(); i++)
				delay[i] = d;
		}
		
		void clear_frames ( bool destroy=false )
		{
			frames.clear();
			delay.clear();
			#if USE_SDL2
				if (destroy)
					destroy_textures();
				texture.clear();
			#endif
		}

		#ifndef USE_SDL2
			void add_frame ( SDL_Rect src, int d )
			{
				index = 0;
				frames.push_back(src);
				set_delay(delay.size() + 1, d);
			}
		#else
			SDL_Texture * get_texture ( int i )
			{
				if (texture.size() > 0 && i < texture.size())
					return texture[i];
				
				return NULL;
			}
			
			void destroy_textures (  )
			{
				vector <SDL_Texture *> t;
				
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
			
			bool has_texture ( SDL_Texture * t )
			{
				if (!t)
					return false;
				
				for (int i(0); i < texture.size(); i++)
					if (t == texture.at(i))
						return true;
				
				return false;
			}
			
			void add_frame ( SDL_Texture * t, SDL_Rect src, int d )
			{
				index = 0;
				texture.push_back(t);
				frames.push_back(src);
				set_delay(delay.size() + 1, d);
			}
		#endif
		
		bool set_index ( int i )
		{
			if (i >= 0 && i < frames.size())
			{
				index = i;
				timer.reset();
				return true;
			}
			
			return false;
		}
		
		int get_index (  )
		{
			return index;
		}
		
		SDL_Rect get_frame ( int i )
		{
			if (i < 0 || i >= frames.size())
				return (SDL_Rect){0,0,0,0};
			
			return frames[i];
		}
		
		SDL_Rect get_curr_frame (  )
		{
			return frames[index];
		}
		
		#ifndef USE_SDL2
			void draw ( int x, int y, SDL_Surface * screen )
		#else
			void draw ( int x, int y, SDL_Renderer * renderer )
		#endif
		{
			SDL_Rect dest, source;
			dest.x = x;
			dest.y = y;
			source = frames[index];
			
			dest.w = source.w;
			dest.h = source.h;
			
			#ifndef USE_SDL2
				if (surface)
					SDL_BlitSurface(surface, &source, screen, &dest);
			#else
				if (texture.size() > 0 && texture.at(get_index()))
					SDL_RenderCopy(renderer, texture.at(get_index()), &source, &dest);
			#endif
		}
		
		#ifndef USE_SDL2
			void draw ( int x, int y, CCamera * cam, SDL_Surface * screen )
		#else
			void draw ( int x, int y, CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			SDL_Rect dest, source;
			source = frames[index];
			
			SVect pos = cam->get_position();
			SDL_Rect dim = cam->get_dimension();

			dest.x = x + dim.x;
			dest.y = y + dim.y;
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
				if (surface)
					SDL_BlitSurface(surface, &source, screen, &dest);
			#else
				if (texture.size() > 0 && texture.at(get_index()))
					SDL_RenderCopy(renderer, texture.at(get_index()), &source, &dest);
			#endif
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 1:
				case 2:
				case 3:
					if (frames.size() == 0 || delay.size() == 0)
						throw "CAnimation: animação sem frames\n";

					timer.update();
					if (timer.steps() >= delay[index])
					{
						timer.reset();
						index++;
						if (index >= frames.size())
						{
							if (repeat)
							{
								index = 0;
								set_state(3); // termina e repete a animação
								break;
							}
							else
							{
								index = frames.size() - 1;
								set_state(4); // terminou a animação e fica parado
								break;
							}
						}
						
						set_state(2); // novo frame
						break;
					}

					set_state(1); // rodando
					break;

				default:
					break;
			}
			
			return get_state();
		}
};

#endif

