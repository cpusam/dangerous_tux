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

#include "sdl.hpp"
#include "vect.hpp"

class Movable
{
	protected:
		Vect pos; // posição
		Vect vel; // velocidade
		Vect acc; // aceleração
		SDL_Rect dim; // posição e dimensão

	public:
		Movable (  )
		{
			dim = (SDL_Rect)
			{
				0,0,1,1
			};
		}

		// remover esses dois
		virtual void set_pos ( Vect p );
		virtual Vect get_pos (  );
		
		virtual void setPosition ( Vect p );

		virtual Vect getPosition (  );

		virtual void set_vel ( Vect v );
		virtual void set_vel_x ( float x );
		virtual void set_vel_y ( float y );

		virtual Vect get_vel (  );
		virtual float get_vel_x (  );
		virtual float get_vel_y (  );

		virtual void set_acc ( Vect a );
		virtual void set_acc_x ( float x );
		virtual void set_acc_y ( float y );

		virtual Vect get_acc (  );
		virtual float get_acc_x (  );
		virtual float get_acc_y (  );

		virtual void set_dim ( SDL_Rect d );

		virtual SDL_Rect get_dim (  );
};

#endif

