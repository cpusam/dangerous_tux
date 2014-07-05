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

#ifndef BUTTON_HPP
#define BUTTON_HPP

/*
	Botão simples de 4 estados, normal, selecionado, pressionado e solto depois de press
*/
class CSimpleButton: public CWidget
{
	protected:
		void (* callback) ( CWidget * b );
		CLabel * label;

	public:
		Uint32 color1; // cor normal
		Uint32 color2; // cor de selecionado
		Uint32 color3; // cor de pressionado

	public:
		CSimpleButton ( SDL_Rect d )
		{
			label = 0;
			callback = 0;
			color1 = 0xFFFF00FF;
			color2 = 0x00FFFFFF;
			color3 = 0xFF0000FF;
			pos.x = d.x, pos.y = d.y;
			// dimensão padrão
			dim = d;
			set_state(1);
		}
		
		CSimpleButton ( SDL_Rect d, string str )
		{
			label = 0;
			callback = 0;
			color1 = 0xFFFF00FF;
			color2 = 0x00FFFFFF;
			color3 = 0xFF0000FF;
			pos.x = d.x, pos.y = d.y;
			// dimensão padrão
			dim = d;
			set_label(new CLabel(str, (SDL_Color){0,0,0,0})); // por padrão na cor preta
			set_state(1);
		}
		
		~CSimpleButton (  )
		{
			if (label)
				delete label;
		}
		
		void set_callback ( void (* c) ( CWidget * b ) )
		{
			callback = c;
		}
	
		void set_dim ( SDL_Rect d )
		{
			dim = d;
			pos.x = d.x;
			pos.y = d.y;

			if (!label)
				return;

			SVect p;
			SDL_Rect dst;
			dst = label->get_dim();
			// posição do label relativo ao botão
			p.x = (dst.w - d.w)/2.0f;
			p.y = (dst.h - d.h)/2.0f;
			label->set_rel_pos(p);
		}
	
		void set_label ( CLabel * l )
		{
			if (label)
			{
				rem_child(label);
				delete label;
			}

			if (!l)
				return;
			
			label = l;
			add_child(label);

			SVect p;
			SDL_Rect d;
			int w, h;
			d = label->get_dim();
			w = d.w, d.w += 10;
			h = d.h, d.h += 5;
			dim.w = d.w, dim.h = d.h; // tamanho do botão
			// posição do label relativo ao botão
			p.x = (d.w - w)/2.0f;
			p.y = (d.h - h)/2.0f;
			label->set_rel_pos(p);
		}
	
		void input ( SDL_Event & event )
		{
			SVect p;
			switch (event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (get_state() == 2)
							if (event.button.state == SDL_PRESSED)
								set_state(3);
					}
					break;
					
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (get_state() == 3)
						{
							if (event.button.state == SDL_RELEASED)
								set_state(4);
						}
					}
					break;
				
				case SDL_MOUSEMOTION:
					p.x = event.motion.x;
					p.y = event.motion.y;
					if (pointbox(p, dim))
					{
						if (get_state() == 1)
							set_state(2);
					}
					else
					{
						set_state(1);
					}
					break;
				default:
					break;
			}
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 1: // normal
					return 1;
				case 2: // selecionado
					return 2;
				case 3: // pressionado
					return 3;
				case 4: // solto depois de pressionado
				{
					if (callback)
						callback(this);

					int x, y;
					SVect p;
					SDL_GetMouseState(&x, &y);
					p.x = x, p.y = y;

					if (pointbox(p, dim))
						set_state(2);

					return 4; 
				}
				default:
					break;
			}
			
			return get_state();
		}
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen )
		#else
			void draw ( SDL_Renderer * renderer )
		#endif
		{
			SDL_Rect d = dim;
			switch (get_state())
			{
				case 1:
					#ifndef USE_SDL2
						SDL_FillRect(screen, &d, color1);
					#else
						SDL_SetRenderDrawColor(renderer, (color1 & 0xFF000000) >> 24, (color1 & 0x00FF0000) >> 16, (color1 & 0x0000FF00) >> 8, (color1 & 0x000000FF));
						
						SDL_RenderFillRect(renderer, &d);
					#endif
					break;
				case 2:
					#ifndef USE_SDL2
						SDL_FillRect(screen, &d, color2);
					#else
						SDL_SetRenderDrawColor(renderer, (color2 & 0xFF000000) >> 24, (color2 & 0x00FF0000) >> 16, (color2 & 0x0000FF00) >> 8, (color2 & 0x000000FF));
						
						SDL_RenderFillRect(renderer, &d);
					#endif
					break;
				case 3:
					#ifndef USE_SDL2
						SDL_FillRect(screen, &d, color3);
					#else
						SDL_SetRenderDrawColor(renderer, (color3 & 0xFF000000) >> 24, (color3 & 0x00FF0000) >> 16, (color3 & 0x0000FF00) >> 8, (color3 & 0x000000FF));
						
						SDL_RenderFillRect(renderer, &d);
					#endif
					break;
				default:
					#ifndef USE_SDL2
						SDL_FillRect(screen, &d, color1);
					#else
						SDL_SetRenderDrawColor(renderer, (color1 & 0xFF000000) >> 24, (color1 & 0x00FF0000) >> 16, (color1 & 0x0000FF00) >> 8, (color1 & 0x000000FF));
						
						SDL_RenderFillRect(renderer, &d);
					#endif
					break;
			}
			
			#ifndef USE_SDL2
				child_draw(screen);
			#else
				child_draw(renderer);
			#endif
		}
};

#endif


