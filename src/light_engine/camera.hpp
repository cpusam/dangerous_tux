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

#ifndef CAMERA_HPP
#define CAMERA_HPP

class CCamera
{
	protected:
		SVect position; // posição dentro do jogo
		SVect focus;
		SDL_Rect dimension;
		SDL_Rect limit;
	
	public:
		CCamera ( SDL_Rect d, SDL_Rect l )
		{
			dimension = d;
			limit = l;
			// focus padrão no meio da camera
			focus.x = d.w / 2.0f;
			focus.y = d.h / 2.0f;
			//lookat(SVect(0,0));
			if (limit.w < dimension.w || limit.h < dimension.h)
				cout << "CCamera: atenção limit W ou H menor que dimension W ou H\n";
		}
		
		void lookat ( SVect p )
		{
			position.x = p.x - focus.x;
			position.y = p.y - focus.y;
			
			if (position.x < limit.x)
				position.x = limit.x;
			else if (position.x + dimension.w > limit.x + limit.w)
				position.x = limit.w - dimension.w;
			
			if (position.y < limit.y)
				position.y = limit.y;
			else if (position.y + dimension.h > limit.x + limit.h)
				position.y = limit.h - dimension.h;
		}
		
		SVect get_position (  )
		{
			return position;
		}
		
		SVect get_focus (  )
		{
			return focus;
		}
		
		void set_position ( SVect p )
		{
			position = p;
			
			if (position.x < limit.x)
				position.x = limit.x;
			else if (position.x + dimension.w > limit.x + limit.w)
				position.x = limit.w - dimension.w;
			
			if (position.y < limit.y)
				position.y = limit.y;
			else if (position.y + dimension.h > limit.x + limit.h)
				position.y = limit.h - dimension.h;
		}
		
		void set_focus ( SVect f )
		{
			focus = f;
		}
		
		SDL_Rect get_dimension (  )
		{
			return dimension;
		}
		
		SDL_Rect get_limit (  )
		{
			return limit;
		}
		
		void set_limit ( SDL_Rect l )
		{
			limit = l;
		}
		
};

#endif

