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

#ifndef CHORA_MOVABLE_HPP
#define CHORA_MOVABLE_HPP

#include "chora.hpp"
#include "vect.hpp"

class CMovable
{
	protected:
		SVect pos; // posição
		SVect vel; // velocidade
		SVect acc; // aceleração
		SDL_Rect dim; // posição e dimensão

	public:
		CMovable (  )
		{
			dim = (SDL_Rect){0,0,1,1};
		}
		
		void set_pos ( SVect p );
		
		SVect get_pos (  );
		
		void set_vel ( SVect v );
		
		SVect get_vel (  );
		
		void set_acc ( SVect a );
		
		SVect get_acc (  );
		
		void set_dim ( SDL_Rect d );
		
		SDL_Rect get_dim (  );
};

#endif
