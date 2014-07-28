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

#ifndef CHORA_VECT_HPP
#define CHORA_VECT_HPP

#include <cmath>

struct SVect
{
	float x;
	float y;
	
	SVect ( )
	{
		x = y = 0;
	}
	
	SVect ( float _x, float _y )
	{
		x = _x;
		y = _y;
	}
	
	SVect operator + ( const SVect & b )
	{
		return SVect(x + b.x, y + b.y);
	}
	
	SVect operator += ( const SVect & b)
	{
		x += b.x;
		y += b.y;
		return *this;
	}
	
	SVect operator - (  )
	{
		return SVect(-x, -y);
	}
	
	SVect operator - ( const SVect & b )
	{
		return SVect(x - b.x, y - b.y);
	}
	
	SVect operator -= ( const SVect & b )
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}
	
	SVect operator * ( const SVect & b )
	{
		return SVect(x * b.x, y * b.y);
	}
	
	SVect operator * ( float s )
	{
		return SVect(x * s, y * s);
	}
	
	SVect operator / ( float s )
	{
		return SVect(x / s, y / s);
	}
	
	SVect operator *= ( const SVect & b )
	{
		x *= b.x;
		y *= b.y;
		return *this;
	}
	
	SVect operator *= ( float s )
	{
		x *= s;
		y *= s;
		return *this;
	}
	
	SVect operator /= ( float s )
	{
		x /= s;
		y /= s;
		return *this;
	}
	
	float length (  )
	{
		return sqrt(x * x + y * y);
	}
	
	SVect normalize (  )
	{
		float h = sqrt(x * x + y * y);
		x /= h;
		y /= h;
		return *this;
	}
	
	SVect zero (  )
	{
		x = y = 0.0f;
		return *this;
	}
	
	SVect set ( float _x, float _y )
	{
		x = _x;
		y = _y;
		return *this;
	}
};

#endif
