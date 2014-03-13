#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

class CBackground
{
	protected:
		#ifndef USE_SDL2
			SDL_Surface * surface;
		#else
			SDL_Texture * texture;
		#endif
	
	public:
		#ifndef USE_SDL2
			CBackground ( SDL_Surface * s=0 )
			{
				surface = s;
			}
		#else
			CBackground ( SDL_Texture * t=0 )
			{
				texture = t;
			}
		#endif
		
		~CBackground (  )
		{
			#ifndef USE_SDL2
				if (surface)
					SDL_FreeSurface(surface);
			#else
				if (texture)
					SDL_DestroyTexture(texture);
			#endif
		}
		
		#ifndef USE_SDL2
			SDL_Surface * get_surface (  )
			{
				return surface;
			}
			
			void set_surface ( SDL_Surface * s )
			{
				if (surface)
					SDL_FreeSurface(surface);
				
				surface = s;
			}
		#else
			SDL_Texture * get_texture (  )
			{
				return texture;
			}
			
			void set_texture ( SDL_Texture * t )
			{
				if (texture)
					SDL_DestroyTexture(texture);
				
				texture = t;
			}
		#endif
		
		/*
			scrolling nos eixos x e y mas limitado pelas bordas da surface
		*/
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			SVect p;
			SDL_Rect d, src;
			int w, h;
			
			#ifndef USE_SDL2
				if (surface == 0)
					return;
				
				w = surface->w;
				h = surface->h;
			#else
				if (!texture)
					return;
				
				SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			#endif
			
			p = cam->get_position();
			d = cam->get_dimension();
			src = d;
			
			src.x = int(p.x);
			src.y = int(p.y);
			
			if (p.x < 0)
			{
				src.x = 0;
			}
			else if (p.x + d.w > w)
			{
				src.x = w - d.w;
			}
			
			if (p.y < 0)
			{
				src.y = 0;
			}
			else if (p.y + d.h > h)
			{
				src.y = h - d.h;
			}
			
			#ifndef USE_SDL2
				SDL_BlitSurface(surface, &src, screen, &d);
			#else
				SDL_RenderCopy(renderer, texture, &src, &d);
			#endif
		}
		// apenas um scrolling horizontal
		#ifndef USE_SDL2
			void draw_hor ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw_hor ( CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			SVect p;
			SDL_Rect d, dim, src;
			int w, h;
			
			#ifndef USE_SDL2
				if (!surface)
					return;
				w = surface->w;
				h = surface->h;
			#else
				if (!texture)
					return;
				
				SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			#endif
			
			p = cam->get_position();
			d = dim = cam->get_dimension();
			src = d;
			
			if (p.x < 0)
			{
				src.y = int(p.y);
				if (p.y < 0)
					src.y = 0;
				else if (p.y + dim.h > h)
					src.y = h - dim.h;

				if (int(p.x) % w < -dim.w)
				{
					src.x = w + int(p.x) % w;
					src.w = dim.w;
					d.x = dim.x;
				}
				else
				{
					src.x = 0;
					src.w = dim.w + int(p.x) % w;
					d.x = dim.x - int(p.x) % w;
				}
				#ifndef USE_SDL2
					SDL_BlitSurface(surface, &src, screen, &d);

					src.x = surface->w + int(p.x) % surface->w;
					src.w = dim.w;
					d.x = dim.x;
					SDL_BlitSurface(surface, &src, screen, &d);
				#else
					SDL_RenderCopy(renderer, texture, &src, &d);
					
					src.x = w + int(p.x) % w;
					src.w = dim.w;
					d.x = dim.x;
					SDL_RenderCopy(renderer, texture, &src, &d);
				#endif
			}
			else
			{
				src.y = int(p.y);
				if (p.y < 0)
					src.y = 0;
				else if (p.y + dim.h > h)
					src.y = h - dim.h;

				src.x = int(p.x) % w;
				src.w = dim.w;
				d.x = dim.x;
				#ifndef USE_SDL2
					SDL_BlitSurface(surface, &src, screen, &d);

					if (int(p.x) % w > w - dim.w)
					{
						src.x = 0;
						src.w = int(p.x) % w - (w - dim.w);
						d.x = dim.x + w - int(p.x) % w;
						SDL_BlitSurface(surface, &src, screen, &d);
					}
				#else
					SDL_RenderCopy(renderer, texture, &src, &d);
					
					if (int(p.x) % w > w - dim.w)
					{
						src.x = 0;
						src.w = int(p.x) % w - (w - dim.w);
						d.x = dim.x + w - int(p.x) % w;
						SDL_RenderCopy(renderer, texture, &src, &d);
					}
				#endif
			}
		}
};

class CAnimatedBackground: private CStateMachine
{
	protected:
		CAnimation anim[2];

	public:
		#ifndef USE_SDL2
			CAnimatedBackground ( SDL_Surface * s )
			{
				anim[0].surface = anim[1].surface = s;
				set_state(1);
			}
			
			~CAnimatedBackground (  )
			{
				if (anim[0].surface)
					SDL_FreeSurface(anim[0].surface);
			}
		#else
			CAnimatedBackground (  )
			{
				set_state(1);
			}
			
			~CAnimatedBackground (  )
			{
				anim[0].destroy_textures();
			}
		#endif
		
		#ifndef USE_SDL2
		void add_frame ( SDL_Rect f, int d )
		{
			anim[0].add_frame(f, d);
			anim[1].add_frame(f, d);
		}
		#else
		void add_frame ( SDL_Texture *t, SDL_Rect f, int d )
		{
			anim[0].add_frame(t, f, d);
			anim[1].add_frame(t, f, d);
		}
		#endif
		
		
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
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			SVect p;
			SDL_Rect d, src;
			int w, h;
			
			#ifndef USE_SDL2
				if (!anim[0].surface)
					return;
				
				w = anim[0].surface->w;
				h = anim[0].surface->h;
			#else
				if (!anim[0].get_texture(0))
					return;
				
				SDL_QueryTexture(anim[0].get_texture(0), NULL, NULL, &w, &h);
			#endif
			
			p = cam->get_position();
			d = cam->get_dimension();
			src = d;
			
			src.x = int(p.x);
			src.y = int(p.y);
			
			if (p.x < 0)
			{
				src.x = 0;
			}
			else if (p.x + d.w > w)
			{
				src.x = w - d.w;
			}
			
			if (p.y < 0)
			{
				src.y = 0;
			}
			else if (p.y + d.h > h)
			{
				src.y = h - d.h;
			}
			
			#ifndef USE_SDL2
				SDL_BlitSurface(anim[0].surface, &src, screen, &d);
			#else
				SDL_RenderCopy(renderer, anim[0].get_texture(0), &src, &d);
			#endif
		}
		
		#ifndef USE_SDL2
			void draw_hor ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw_hor ( CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			SVect p;
			SDL_Rect d, dim, src, surf;
			
			#ifndef USE_SDL2
				if (!anim[0].surface)
					return;
			#else
				if (!anim[0].get_texture(0))
					return;
			#endif
			
			p = cam->get_position();
			d = dim = cam->get_dimension();
			surf = src = anim[0].get_curr_frame();
			
			src.y = surf.y + int(p.y) % surf.h;
			if (p.y < 0)
				src.y = surf.y;
			else if (p.y + dim.h >= surf.h)
				src.y = surf.y + (surf.h - dim.h);

			src.x = surf.x + int(p.x) % surf.w;
			src.w = dim.w;
			src.h = dim.h;
			d.x = dim.x;
			#ifndef USE_SDL2
				SDL_BlitSurface(anim[0].surface, &src, screen, &d);

				if (int(p.x) % surf.w > surf.w - dim.w)
				{
					src.x = surf.x;
					src.w = int(p.x) % surf.w - (surf.w - dim.w);
					d.x = dim.x + surf.w - int(p.x) % surf.w;
					SDL_BlitSurface(anim[0].surface, &src, screen, &d);
				}
			#else
				SDL_RenderCopy(renderer, anim[0].get_texture(0), &src, &d);

				if (int(p.x) % surf.w > surf.w - dim.w)
				{
					src.x = surf.x;
					src.w = int(p.x) % surf.w - (surf.w - dim.w);
					d.x = dim.x + surf.w - int(p.x) % surf.w;
					SDL_RenderCopy(renderer, anim[0].get_texture(0), &src, &d);
				}
			#endif
		}
};

#endif

