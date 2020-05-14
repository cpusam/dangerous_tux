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

#ifndef CHORA_TEXTINPUT_HPP
#define CHORA_TEXTINPUT_HPP

#include <map>

#include "label.hpp"
#include "../include/animation.hpp"
#include "../include/Exception.hpp"
#include "../game/Key.hpp"

/*
	GuiTextInput usada para escrever o que o usuário digitar
	O "cursor" é sempre posicionado no final da std::string
	-colocar para setar a posição do cursor com as setas direita e esquerda
	-colocar para setar a posição do cursor com o mouse
	-colocar os caracteres que precisão de shift para ser usados
*/
class GuiTextInput: public GuiLabel
{
	private:
		bool locked;//trancado para novos caracteres
		bool remakeTexture;//sinal para refazer a textura
		bool caps_lock;
		bool shift_key;
		int textsize; // tamanho da fonte em pixels (largura de uma letra)
		int strsize; // tamanho máximo da str (contando o '\0')
		int time_change; //tempo em milisegundos para o cursor piscar
		int cursorStart;//inicio do curso (indice do caractere)
		SDL_Rect cursor;
		SDL_Rect box;
		STimer timer;

	public:
		Uint32 cursor_color;
		std::map<std::string, Key> controls;

	protected:
		Vect cursorPos;
		using Widget::set_pos;
		using Widget::set_rel_pos;

	public:
		GuiTextInput ( int fontsize, SDL_Color c, int ss=10, std::string fontName="=>default" );
		virtual ~GuiTextInput();

		SDL_Rect get_box (  );

		void show ( bool s=true );

		void lock (  );
		void unlock (  );

		/*
			Define a dimensão do cursor
		*/
		void set_cursor_size ( int w, int h );

		void set_pos ( Vect p );

		void set_rel_pos ( Vect p );

		void input ( SDL_Event & event );

		int update (  );

		void draw ( SDL_Renderer * renderer );
};

#endif

