#ifndef LABEL_HPP
#define LABEL_HPP

class CLabel: public CWidget
{
	private:
		SDL_Color color;
		SDL_Surface * surface;
	protected:
		string str;

	protected:
		CLabel (  )
		{
			color = (SDL_Color){0,0,0,0};
			surface = 0;
		}
	public:
		CLabel ( string s, SDL_Color c )
		{
			color = c;
			surface = 0;
			if (s != "")
				set_str(s);
			else
				str = "";
		}
		
		~CLabel (  )
		{
			if (surface)
				SDL_FreeSurface(surface);
		}
		
		protected:
			void str_to_surface ( string s )
			{
				if (surface)
					SDL_FreeSurface(surface);

				surface = CWriter::instance()->render_text(s, color, UTF8_TEXT);
				if (!surface)
					throw SDL_GetError();
				
				dim.x = pos.x + rel_pos.x, dim.y = pos.y + rel_pos.y;
				dim.w = surface->w, dim.h = surface->h;
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

			void set_surface ( SDL_Surface * s )
			{
				if (surface)
					SDL_FreeSurface(surface);
			
				surface = s;
				dim.w = surface->w;
				dim.h = surface->h;
			}

			string get_str (  )
			{
				return str;
			}

			SDL_Surface * get_surface (  )
			{
				return surface;
			}

			void draw ( SDL_Surface * screen )
			{
				if (!visible)
					return;

				SDL_Rect d = dim;
				if (surface)
					SDL_BlitSurface(surface, NULL, screen, &d);
				child_draw(screen);
			}
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
};

/*
	CTextInput usada para escrever o que o usuário digitar
	O "cursor" é sempre posicionado no final da string
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
				throw "CTextInput: tamanho da string inválido\n";
			
			caps_lock = false;
			shift_key = false;
			count = 0;
			
			str_to_surface("a");
			textsize = get_surface()->w;
			str_to_surface("");
			
			strsize = ss;
			cursor.x = pos.x + rel_pos.x;
			cursor.y = pos.y + rel_pos.y;
			cursor.w = 5;
			cursor.h = fontsize;
			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				cursor_color = 0x0000FF;
			#else
				cursor_color = 0xFF0000;
			#endif
		}
		
		/*
			Define a dimensão do cursor
		*/
		void set_cursor_size ( int w, int h )
		{
			cursor.w = w;
			cursor.h = h;
		}
		
		void set_pos ( SVect p )
		{
			CWidget::set_pos(p);
			cursor.x = pos.x + rel_pos.x + get_surface()->w;
			cursor.y = pos.y + rel_pos.y;
		}
		
		void set_rel_pos ( SVect p )
		{
			CWidget::set_rel_pos(p);
			cursor.x = pos.x + rel_pos.x + get_surface()->w;
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
						cursor.x = pos.x + rel_pos.x + get_surface()->w;
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
						cursor.x = pos.x + rel_pos.x + get_surface()->w;
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
		
		void draw ( SDL_Surface * screen )
		{
			if (!visible)
				return;

			SDL_Rect d;

			CLabel::draw(screen);
			
			if ((++count) % 10 < 5)
			{
				d = cursor;
				SDL_FillRect(screen, &d, cursor_color);
			}
			
			child_draw(screen);
		}
};

#endif

