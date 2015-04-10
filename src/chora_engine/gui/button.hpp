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

#ifndef CHORA_BUTTON_HPP
#define CHORA_BUTTON_HPP

#include "widget.hpp"
#include "label.hpp"
#include "../collision.hpp"

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
		
		CSimpleButton ( SDL_Rect d, std::string str )
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
		
		virtual void set_callback ( void (* c) ( CWidget * b ) );
	
		virtual void set_dim ( SDL_Rect d );
	
		virtual void set_label ( CLabel * l );
			
		virtual void input ( SDL_Event & event );
		
		virtual int update (  );
		
		#ifndef USE_SDL2
			virtual void draw ( SDL_Surface * screen );
		#else
			virtual void draw ( SDL_Renderer * renderer );
		#endif
};

#endif


