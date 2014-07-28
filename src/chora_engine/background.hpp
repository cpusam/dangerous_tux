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

<<<<<<< HEAD
#ifndef CHORA_BACKGROUND_HPP
#define CHORA_BACKGROUND_HPP

#include "chora.hpp"
#include "vect.hpp"
#include "camera.hpp"
#include "animation.hpp"
=======
#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813

/*
	NOTA: quando a posição é negativa não está havendo movimento do background
*/

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
<<<<<<< HEAD
			SDL_Surface * get_surface (  );
			
			bool set_surface ( SDL_Surface * s );
		#else
			SDL_Texture * get_texture (  );
			
			bool set_texture ( SDL_Texture * t );
=======
			SDL_Surface * get_surface (  )
			{
				return surface;
			}
			
			bool set_surface ( SDL_Surface * s )
			{
				if (surface && surface != s)
					SDL_FreeSurface(surface);
				
				surface = s;
				
				return surface != 0;
			}
		#else
			SDL_Texture * get_texture (  )
			{
				return texture;
			}
			
			bool set_texture ( SDL_Texture * t )
			{
				if (texture && texture != t)
					SDL_DestroyTexture(texture);
				
				texture = t;
				
				return (texture != 0);
			}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		#endif
		
		/*
			scrolling nos eixos x e y mas limitado pelas bordas da surface
		*/
		
		#ifndef USE_SDL2
<<<<<<< HEAD
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
		
		// apenas um scrolling horizontal
		#ifndef USE_SDL2
			void draw_hor ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw_hor ( CCamera * cam, SDL_Renderer * renderer );
		#endif
=======
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
			
			if (cam)
			{
				p = cam->get_position();
				d = cam->get_dimension();
				src = d;
			}
			else
			{
				#ifndef USE_SDL2
					d = (SDL_Rect){0,0,screen->w,screen->h};
				#else
					int rw, rh;
					SDL_RenderGetLogicalSize(renderer, &rw, &rh);
					d = (SDL_Rect){0,0,rw,rh};
				#endif
				src = d;
			}
			
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
			
			src.y = int(p.y);
			if (p.y < 0)
				src.y = 0;
			else if (p.y + dim.h > h)
				src.y = h - dim.h;
			
			if (p.x < 0)
			{
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
				// para o destino a esquerda
				src.x = int(p.x) % w;
				src.w = w - int(p.x) % w;
				d.x = dim.x;
				d.w = src.w;
				#ifndef USE_SDL2
					SDL_BlitSurface(surface, &src, screen, &d);

					if (int(p.x) % w > w - dim.w)
					{
						src.x = 0;
						src.w = int(p.x) % w - (w - dim.w);
						d.x = dim.x + w - int(p.x) % w;
						d.w = src.w;
						SDL_BlitSurface(surface, &src, screen, &d);
					}
				#else
					SDL_RenderCopy(renderer, texture, &src, &d);
					
					if (int(p.x) % w > w - dim.w)
					{
						src.x = 0;
						src.w = int(p.x) % w - (w - dim.w);
						d.x = dim.x + w - int(p.x) % w;
						d.w = src.w;
						SDL_RenderCopy(renderer, texture, &src, &d);
					}
				#endif
			}
		}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		
		// NOTE: falta testar, precisa refazer
		// apenas um scrolling vertical
		#ifndef USE_SDL2
<<<<<<< HEAD
			void draw_ver ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw_ver ( CCamera * cam, SDL_Renderer * renderer );
		#endif
=======
			void draw_ver ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw_ver ( CCamera * cam, SDL_Renderer * renderer )
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
			
			if (p.y < 0)
			{
				src.x = int(p.x);
				if (p.x < 0)
					src.x = 0;
				else if (p.x + dim.w > w)
					src.w = w - dim.w;

				if (int(p.y) % h < -dim.h)
				{
					src.y = h + int(p.y) % h;
					src.h = dim.h;
					d.y = dim.y;
				}
				else
				{
					src.y = 0;
					src.h = dim.h + int(p.y) % h;
					d.y = dim.y - int(p.x) % h;
				}
				#ifndef USE_SDL2
					SDL_BlitSurface(surface, &src, screen, &d);

					src.y = surface->h + int(p.y) % surface->h;
					src.h = dim.h;
					d.y = dim.y;
					SDL_BlitSurface(surface, &src, screen, &d);
				#else
					SDL_RenderCopy(renderer, texture, &src, &d);
					
					src.y = h + int(p.y) % h;
					src.h = dim.h;
					d.y = dim.y;
					SDL_RenderCopy(renderer, texture, &src, &d);
				#endif
			}
			else
			{
				src.x = int(p.x);
				if (p.x < 0)
					src.x = 0;
				else if (p.x + dim.w > w)
					src.x = w - dim.w;

				src.y = int(p.y) % h;
				src.h = dim.h;
				d.y = dim.y;
				#ifndef USE_SDL2
					SDL_BlitSurface(surface, &src, screen, &d);

					if (int(p.y) % h > h - dim.h)
					{
						src.y = 0;
						src.h = int(p.y) % h - (h - dim.h);
						d.h = dim.h + h - int(p.y) % h;
						SDL_BlitSurface(surface, &src, screen, &d);
					}
				#else
					SDL_RenderCopy(renderer, texture, &src, &d);
					
					if (int(p.y) % h > h - dim.h)
					{
						src.y = 0;
						src.h = int(p.y) % h - (h - dim.h);
						d.y = dim.y + h - int(p.y) % h;
						SDL_RenderCopy(renderer, texture, &src, &d);
					}
				#endif
			}
		}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

class CAnimatedBackground: private CStateMachine
{
	protected:
		CAnimation anim[2];

	public:
		CAnimatedBackground (  )
		{
			set_state(1);
		}

		~CAnimatedBackground (  )
		{
			#ifndef USE_SDL2
				anim[0].destroy_surfaces();
			#else
				anim[0].destroy_textures();
			#endif
		}
		
		#ifndef USE_SDL2
<<<<<<< HEAD
			void add_frame ( SDL_Surface * s, SDL_Rect f, int d );
		#else
			void add_frame ( SDL_Texture *t, SDL_Rect f, int d );
		#endif
		
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer );
		#endif
		
		#ifndef USE_SDL2
			void draw_hor ( CCamera * cam, SDL_Surface * screen );
		#else
			void draw_hor ( CCamera * cam, SDL_Renderer * renderer );
		#endif
=======
		void add_frame ( SDL_Surface * s, SDL_Rect f, int d )
		{
			anim[0].add_frame(s, f, d);
			anim[1].add_frame(s, f, d);
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
				if (!anim[0].get_surface(0))
					return;
				
				w = anim[0].get_surface(0)->w;
				h = anim[0].get_surface(0)->h;
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
				SDL_BlitSurface(anim[0].get_surface(0), &src, screen, &d);
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
				if (!anim[0].get_surface(0))
					return;
			#else
				if (!anim[0].get_texture(0))
					return;
			#endif
			
			p = cam->get_position();
			d = dim = cam->get_dimension();
			surf = src = anim[0].get_curr_frame().get_source();
			
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
				SDL_BlitSurface(anim[0].get_surface(0), &src, screen, &d);

				if (int(p.x) % surf.w > surf.w - dim.w)
				{
					src.x = surf.x;
					src.w = int(p.x) % surf.w - (surf.w - dim.w);
					d.x = dim.x + surf.w - int(p.x) % surf.w;
					SDL_BlitSurface(anim[0].get_surface(0), &src, screen, &d);
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
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

#endif

