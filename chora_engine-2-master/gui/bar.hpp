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

#ifndef BAR_HPP
#define BAR_HPP

#include "widget.hpp"

class GuiBar: public Widget
{
	protected:
		float size;
		float full_size;
		int ori; // 0 - horizontal, 1 - vertical
		int dir; // 0 da direita para esquerda ou de baixo pra cima, 1 da esquerda para direita ou de cima para baixo
	public:
		Uint32 color_bg; // cor de background
		Uint32 color_bar; // cor da barra

	public:
		GuiBar ( float s_max, int w, int h )
		{
			size = full_size = s_max;
			color_bg = 0x000000FF;
			color_bar = 0xFFFFFFFF;
			dim.w = w;
			dim.h = h;
			ori = 0;
			dir = 0;
		}

		void set_orientation ( int o );

		void set_direction ( int d );

		void set_size ( float s );

		void reset (  );

		void add ( float s );

		void draw ( SDL_Renderer * renderer );
};

#endif

