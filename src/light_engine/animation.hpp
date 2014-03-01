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

	public:
		SDL_Surface * surface;
		
	public:
		CAnimation (  )
		{
			set_state(1);
			repeat = true;
			index = 0;
			timer.start();
			surface = 0;
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
		
		void clear_frames (  )
		{
			frames.clear();
			delay.clear();
		}

		void add_frame ( SDL_Rect src, int d )
		{
			index = 0;
			frames.push_back(src);
			set_delay(delay.size() + 1, d);
		}
		
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
		
		void draw ( int x, int y, SDL_Surface * screen )
		{
			SDL_Rect dest, source;
			dest.x = x;
			dest.y = y;
			source = frames[index];
			dest.w = source.w;
			dest.h = source.h;
			
			if (surface)
				SDL_BlitSurface(surface, &source, screen, &dest);
		}
		
		void draw ( int x, int y, CCamera * cam, SDL_Surface * screen )
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
			
			if (surface)
				SDL_BlitSurface(surface, &source, screen, &dest);
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 0:
					return 0; // animação parada
				case 1:
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
								index = 0;
							else
								index = frames.size() - 1;

							return 3; // terminou a animação
						}
						
						return 2; // novo frame
					}

					return 1; // rodando
			}
			
			return get_state();
		}
};

#endif

