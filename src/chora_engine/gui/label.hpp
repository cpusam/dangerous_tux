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

#ifndef LABEL_HPP
#define LABEL_HPP

<<<<<<< HEAD
#include "widget.hpp"
#include "writer.hpp"

#include <sstream>

=======
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
class CLabel: public CWidget
{
	private:
		SDL_Color color;
		#ifndef USE_SDL2
			SDL_Surface * surface;
		#else
			SDL_Texture * texture;
		#endif
		
	protected:
<<<<<<< HEAD
		std::string str;
=======
		string str;
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813

	protected:
		CLabel (  )
		{
			color = (SDL_Color){0,0,0,255};
			#ifndef USE_SDL2
				surface = 0;
			#else
				texture = 0;
			#endif
		}

	public:
<<<<<<< HEAD
		CLabel ( std::string s, SDL_Color c )
=======
		CLabel ( string s, SDL_Color c )
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		{
			color = c;
			
			#ifndef USE_SDL2
				surface = 0;
			#else
				texture = 0;
			#endif
			/*
			if (s != "")
				set_str(s);
			else
				str = "";
			*/
			if (s == "")
				s = " ";
			
			set_str(s);
		}
		
		~CLabel (  )
		{
			#ifndef USE_SDL2
				if (surface)
					SDL_FreeSurface(surface);
			#else
				if (texture)
					SDL_DestroyTexture(texture);
			#endif
		}
		
		protected:
<<<<<<< HEAD
			void str_to_surface ( std::string s );
		
		public:
			void set_color ( SDL_Color c );

			void set_str ( std::string s );
			
			#ifndef USE_SDL2
				SDL_Surface * get_surface (  );

				void set_surface ( SDL_Surface * s );
			#else
				SDL_Texture * get_texture (  );
				
				void set_texture ( SDL_Texture * t );
				
				int get_texture_width (  );
				
				int get_texture_height (  );
			#endif

			std::string get_str (  );

			#ifndef USE_SDL2
				void draw ( SDL_Surface * screen );
			#else
				void draw ( SDL_Renderer * renderer );
			#endif
=======
			void str_to_surface ( string s )
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
		
		public:
			void set_color ( SDL_Color c )
			{
				color = c;
				str_to_surface(str);
			}

			void set_str ( string s )
			{
				if (s == str)
					return;
			
				str = s;
			
				str_to_surface(s);
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
					dim.w = surface->w;
					dim.h = surface->h;
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
				
				int get_texture_width (  )
				{
					int w;
					
					if (!texture)
						return 0;
						
					SDL_QueryTexture(texture, NULL, NULL, &w, NULL);
					return w;
				}
				
				int get_texture_height (  )
				{
					int h;
					
					if (!texture)
						return 0;
					
					SDL_QueryTexture(texture, NULL, NULL, NULL, &h);
					return h;
				}
			#endif

			string get_str (  )
			{
				return str;
			}

			#ifndef USE_SDL2
				void draw ( SDL_Surface * screen )
			#else
				void draw ( SDL_Renderer * renderer )
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
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

/*
	NOTA: por enquanto apenas números inteiros
*/
class CLabelNumber: public CLabel
{
	protected:
		float value;
		float left_zero; // número máximo de zeros a esquerda
	protected:
		using CLabel::set_str;

	public:
		CLabelNumber ( float v, SDL_Color c, int lz=0 )
		{
			set_color(c);
			value = -1;
			left_zero = lz;
			set_value(v);
		}
		
		protected:
<<<<<<< HEAD
			void proc_value ( float v ); // para ser usada internamente
		
		public:
			void set_left_zero ( int lz );
		
			bool set_value ( float v );
=======
			void proc_value ( float v ) // para ser usada internamente
			{
				int n = v, a, i;
				string s, aux;
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
		
		public:
			void set_left_zero ( int lz )
			{
				if (lz > -1 && lz != left_zero)
				{
					left_zero = lz;			
					proc_value(value);
				}
			}
		
			bool set_value ( float v )
			{
				if (value == v)
					return false;
			
				proc_value(int(v));
				value = v;
			
				return true;
			}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

/*
	CTextInput usada para escrever o que o usuário digitar
<<<<<<< HEAD
	O "cursor" é sempre posicionado no final da std::string
=======
	O "cursor" é sempre posicionado no final da string
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
	-colocar para setar a posição do cursor com as setas direita e esquerda
	-colocar para setar a posição do cursor com o mouse
	-colocar os caracteres que precisão de shift para ser usados
*/
class CTextInput: public CLabel
{
	private:
		bool caps_lock;
		bool shift_key;
		int count;
		int textsize; // tamanho da fonte em pixels
		int strsize; // tamanho máximo da str
		SDL_Rect cursor;
	public:
		Uint32 cursor_color;

	protected:
		using CWidget::set_pos;
		using CWidget::set_rel_pos;

	public:
		CTextInput ( int fontsize, SDL_Color c, int ss=10 ): CLabel("", c)
		{
			if (fontsize < 1)
				throw "CTextInput: tamanho do texto inválido\n";
			
			if (ss < 1)
<<<<<<< HEAD
				throw "CTextInput: tamanho da std::string inválido\n";
=======
				throw "CTextInput: tamanho da string inválido\n";
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
			
			caps_lock = false;
			shift_key = false;
			count = 0;
			
			str_to_surface("a");
			#ifndef USE_SDL2
				textsize = get_surface()->w;
			#else
				textsize = get_texture_width();
			#endif
			str_to_surface("");
			
			strsize = ss;
			cursor.x = pos.x + rel_pos.x;
			cursor.y = pos.y + rel_pos.y;
			cursor.w = 5;
			cursor.h = fontsize;
			cursor_color = 0xFF0000FF;
		}
		
		/*
			Define a dimensão do cursor
		*/
<<<<<<< HEAD
		void set_cursor_size ( int w, int h );
		
		void set_pos ( SVect p );
		
		void set_rel_pos ( SVect p );
		
		void input ( SDL_Event & event );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
=======
		void set_cursor_size ( int w, int h )
		{
			cursor.w = w;
			cursor.h = h;
		}
		
		void set_pos ( SVect p )
		{
			CWidget::set_pos(p);
			#ifndef USE_SDL2
				cursor.x = pos.x + rel_pos.x + get_surface()->w;
			#else
				cursor.x = pos.x + rel_pos.x + get_texture_width();
			#endif
			cursor.y = pos.y + rel_pos.y;
		}
		
		void set_rel_pos ( SVect p )
		{
			CWidget::set_rel_pos(p);
			#ifndef USE_SDL2
				cursor.x = pos.x + rel_pos.x + get_surface()->w;
			#else
				cursor.x = pos.x + rel_pos.x + get_texture_width();
			#endif
			cursor.y = pos.y + rel_pos.y;
		}
		
		void input ( SDL_Event & event )
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
				else if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z)
				{
					if (str.size() < strsize)
					{
						stringstream s;
						s << str;

						if (event.key.keysym.sym >= SDLK_a)
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
			void draw ( SDL_Surface * screen )
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
			void draw ( SDL_Renderer * renderer )
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
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		#endif
};

#endif

