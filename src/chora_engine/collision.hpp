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

#ifndef COLLISION_HPP
#define COLLISION_HPP

bool boudingbox ( SDL_Rect a, SDL_Rect b )
{
	if (a.x > b.x + b.w - 1)	return false;
	if (a.x + a.w < b.x)	return false;
	
	if (a.y > b.y + b.h - 1)	return false;
	if (a.y + a.h < b.y)	return false;
	
	return true;
}

bool pointbox ( SVect p, SDL_Rect b )
{
	if (p.x > b.x + b.w - 1)	return false;
	if (p.x < b.x)			return false;

	if (p.y > b.y + b.h - 1)	return false;
	if (p.y < b.y)			return false;
	
	return true;
}

// verifica se "a" está completamente dentro de "b", mas não o contrário
bool rect_inside ( SDL_Rect a, SDL_Rect b )
{
	if (a.x + a.w > b.x + b.w)	return false;
	if (a.x < b.x)	return false;
	
	if (a.y + a.h > b.y + b.h)	return false;
	if (a.y < b.y)	return false;
	
	return true;
}


#endif


