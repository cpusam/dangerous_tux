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

#ifndef CHORA_LABEL_HPP
#define CHORA_LABEL_HPP

#include "widget.hpp"
#include "writer.hpp"

#include <sstream>

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
		std::string str;

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
		CLabel ( std::string s, SDL_Color c )
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
			void proc_value ( float v ); // para ser usada internamente
		
		public:
			void set_left_zero ( int lz );
		
			bool set_value ( float v );
};

/*
	CTextInput usada para escrever o que o usuário digitar
	O "cursor" é sempre posicionado no final da std::string
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
				throw "CTextInput: tamanho da std::string inválido\n";
			
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
		void set_cursor_size ( int w, int h );
		
		void set_pos ( SVect p );
		
		void set_rel_pos ( SVect p );
		
		void input ( SDL_Event & event );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
};

#endif

