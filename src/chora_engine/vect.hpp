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

#define TO_RADIANS(a) ((a) * M_PI) / 180.0f
#define TO_DEGREES(a) ((a) * 180.0f) / M_PI

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
	
	SVect operator - (  )
	{
		return SVect(-x, -y);
	}
	
	SVect operator - ( const SVect & b )
	{
		return SVect(x - b.x, y - b.y);
	}
	
	/*
	// seria um vetor perpedicular no eixo Z
	SVect operator * ( const SVect & b )
	{
		return SVect(x * b.x, y * b.y);
	}
	*/
	
	float operator * ( const SVect & b )
	{
		return x * b.x + y * b.y;
	}
	
	SVect operator * ( float s )
	{
		return SVect(x * s, y * s);
	}
	
	SVect operator / ( float s )
	{
		return SVect(x / s, y / s);
	}
	
	SVect& operator += ( const SVect & b)
	{
		x += b.x;
		y += b.y;
		return *this;
	}
	
	SVect operator -= ( const SVect & b )
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}
	
	/*
	// seria um vetor perpedicular no eixo Z
	SVect& operator *= ( const SVect & b )
	{
		x *= b.x;
		y *= b.y;
		return *this;
	}
	*/
	
	SVect& operator *= ( float s )
	{
		x *= s;
		y *= s;
		return *this;
	}
	
	float operator *= ( const SVect & b )
	{
		return x * b.x + y * b.y;
	}
	
	SVect& operator /= ( float s )
	{
		x /= s;
		y /= s;
		return *this;
	}
	
	float length (  )
	{
		return sqrt(x * x + y * y);
	}
	
	SVect normal (  )
	{
		float h = sqrt(x * x + y * y);
		return SVect(x / h, y / h);
	}
	
	SVect& normalize (  )
	{
		float h = sqrt(x * x + y * y);
		x /= h;
		y /= h;
		return *this;
	}
	/*
	float angle (  )
	{
		SVect a(x, y);
		
		a.normalize();
		
		return
	}
	*/
	
	SVect& rotate ( float angle )
	{
		float c = cos(angle);
		float s = sin(angle);
		
		float nx = x * c - y * s;
		float ny = x * s + y * c;
		
		x = nx;
		y = ny;
		
		return *this;
	}
	
	float cos_theta ( SVect & b )
	{
		float d = (b.length() * (this)->length());
		float c = ((*this) * b) / d;
		
		if (d == 0 || isnan(c))
			return 1.0f;
		
		return c;
	}
	
	SVect& project ( SVect & b )
	{
		float s = cos_theta(b);

		x *= s;
		y *= s;
		
		return *this;
	}
	
	SVect& to_int (  )
	{
		x = int(x);
		y = int(y);
		return *this;
	}
	
	SVect& zero (  )
	{
		x = y = 0.0f;
		return *this;
	}
	
	SVect& set ( float _x, float _y )
	{
		x = _x;
		y = _y;
		return *this;
	}
};

#endif
