#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

class CBackground
{
	public:
		SDL_Surface * surface;
	
	public:
		CBackground ( SDL_Surface * s=0 )
		{
			surface = s;
		}
		
		/*
			scrolling nos eixos x e y mas limitado pelas bordas da surface
		*/
		void draw ( CCamera * cam, SDL_Surface * screen )
		{
			SVect p;
			SDL_Rect d, src;
			
			if (!surface)
				return;
			
			p = cam->get_position();
			d = cam->get_dimension();
			src = d;
			
			src.x = int(p.x);
			src.y = int(p.y);
			
			if (p.x < 0)
			{
				src.x = 0;
			}
			else if (p.x + d.w > surface->w)
			{
				src.x = surface->w - d.w;
			}
			
			if (p.y < 0)
			{
				src.y = 0;
			}
			else if (p.y + d.h > surface->h)
			{
				src.y = surface->h - d.h;
			}
			
			SDL_BlitSurface(surface, &src, screen, &d);
		}
		// apenas um scrolling horizontal
		void draw_hor ( CCamera * cam, SDL_Surface * screen )
		{
			SVect p;
			SDL_Rect d, dim, src;
			
			if (!surface)
				return;
			
			p = cam->get_position();
			d = dim = cam->get_dimension();
			src = d;
			
			if (p.x < 0)
			{
				src.y = int(p.y);
				if (p.y < 0)
					src.y = 0;
				else if (p.y + dim.h > surface->h)
					src.y = surface->h - dim.h;

				if (int(p.x) % surface->w < -dim.w)
				{
					src.x = surface->w + int(p.x) % surface->w;
					src.w = dim.w;
					d.x = dim.x;
				}
				else
				{
					src.x = 0;
					src.w = dim.w + int(p.x) % surface->w;
					d.x = dim.x - int(p.x) % surface->w;
				}
				
				SDL_BlitSurface(surface, &src, screen, &d);

				src.x = surface->w + int(p.x) % surface->w;
				src.w = dim.w;
				d.x = dim.x;
				SDL_BlitSurface(surface, &src, screen, &d);
			}
			else
			{
				src.y = int(p.y);
				if (p.y < 0)
					src.y = 0;
				else if (p.y + dim.h > surface->h)
					src.y = surface->h - dim.h;

				src.x = int(p.x) % surface->w;
				src.w = dim.w;
				d.x = dim.x;
				SDL_BlitSurface(surface, &src, screen, &d);

				if (int(p.x) % surface->w > surface->w - dim.w)
				{
					src.x = 0;
					src.w = int(p.x) % surface->w - (surface->w - dim.w);
					d.x = dim.x + surface->w - int(p.x) % surface->w;
					SDL_BlitSurface(surface, &src, screen, &d);
				}
			}
		}
};

class CAnimatedBackground: private CStateMachine
{
	protected:
		CAnimation anim[2];

	public:
		CAnimatedBackground ( SDL_Surface * s )
		{
			anim[0].surface = anim[1].surface = s;
			set_state(1);
		}
		
		void add_frame ( SDL_Rect f, int d )
		{
			anim[0].add_frame(f, d);
			anim[1].add_frame(f, d);
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 0:
					break;
				case 1:
					anim[0].update();
					anim[1].update();
					break;
			}
		}
		
		void draw_hor ( CCamera * cam, SDL_Surface * screen )
		{
			SVect p;
			SDL_Rect d, dim, src, surf;
			
			if (!anim[0].surface)
				return;
			
			p = cam->get_position();
			d = dim = cam->get_dimension();
			src = anim[0].get_curr_frame();
			surf = src;
			
			src.y = surf.y + int(p.y) % surf.h;
			if (p.y < 0)
				src.y = surf.y;
			else if (p.y + dim.h >= surf.h)
				src.y = surf.y + (surf.h - dim.h);

			src.x = surf.x + int(p.x) % surf.w;
			src.w = dim.w;
			src.h = dim.h;
			d.x = dim.x;
			SDL_BlitSurface(anim[0].surface, &src, screen, &d);

			if (int(p.x) % surf.w > surf.w - dim.w)
			{
				src.x = surf.x;
				src.w = int(p.x) % surf.w - (surf.w - dim.w);
				d.x = dim.x + surf.w - int(p.x) % surf.w;
				SDL_BlitSurface(anim[0].surface, &src, screen, &d);
			}
		}
};

#endif

