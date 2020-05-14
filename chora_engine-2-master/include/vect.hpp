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
#include <string>

#ifdef _DEBUG_
#include <cstdio>
#endif

#define TO_RADIANS(a) ((a) * M_PI) / 180.0f
#define TO_DEGREES(a) ((a) * 180.0f) / M_PI

struct Vect
{
	float x;
	float y;

	Vect ( )
	{
		x = y = 0;
	}

	Vect ( float _x, float _y )
	{
		x = _x;
		y = _y;
	}

	Vect operator + ( const Vect & b )
	{
		return Vect(x + b.x, y + b.y);
	}

	Vect operator - (  )
	{
		return Vect(-x, -y);
	}

	Vect operator - ( const Vect & b )
	{
		return Vect(x - b.x, y - b.y);
	}

	/*
	// seria um vetor perpedicular no eixo Z
	Vect operator * ( const Vect & b )
	{
		return Vect(x * b.x, y * b.y);
	}
	*/

	float operator * ( const Vect & b )
	{
		return x * b.x + y * b.y;
	}

	Vect operator * ( float s )
	{
		return Vect(x * s, y * s);
	}

	Vect operator / ( float s )
	{
		return Vect(x / s, y / s);
	}

	Vect& operator += ( const Vect & b)
	{
		x += b.x;
		y += b.y;
		return *this;
	}

	Vect operator -= ( const Vect & b )
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}

	/*
	// seria um vetor perpedicular no eixo Z
	Vect& operator *= ( const Vect & b )
	{
		x *= b.x;
		y *= b.y;
		return *this;
	}
	*/

	Vect& operator *= ( float s )
	{
		x *= s;
		y *= s;
		return *this;
	}

	float operator *= ( const Vect & b )
	{
		return x * b.x + y * b.y;
	}

	Vect& operator /= ( float s )
	{
		x /= s;
		y /= s;
		return *this;
	}

	inline static Vect add (Vect a, Vect b)
	{
		return Vect(a.x + b.x, a.y + b.y);
	}

	inline static Vect sub ( Vect a, Vect b )
	{
		return Vect(a.x - b.x, a.y - b.y);
	}

	inline Vect & scale ( float s )
	{
		this->x *= s;
		this->y *= s;
		
		return *this;
	}

	float length (  )
	{
		return sqrt(x * x + y * y);
	}

	Vect normal (  )
	{
		float h = sqrt(x * x + y * y);
		return Vect(x / h, y / h);
	}

	Vect& normalize (  )
	{
		float h = sqrt(x * x + y * y);
		x /= h;
		y /= h;
		return *this;
	}
	/*
	float angle (  )
	{
		Vect a(x, y);

		a.normalize();

		return
	}
	*/

	// angulo em radianos
	Vect& rotate ( float angle )
	{
		float c = cos(angle);
		float s = sin(angle);

		float nx = x * c - y * s;
		float ny = x * s + y * c;

		x = nx;
		y = ny;

		return *this;
	}

	//rotação em graus
	Vect& rotateDegree ( float angle )
	{
		angle = TO_RADIANS(angle);

		return this->rotate(angle);
	}

	float cos_theta ( Vect & b )
	{
		float d = (b.length() * (this)->length());
		if (d == 0)
			return 1.0f;
		
		float c = ((*this) * b) / d;
		if (std::isnan(c))
			return 1;

		return c;
	}

	Vect& project ( Vect & b )
	{
		float s = cos_theta(b);

		x *= s;
		y *= s;

		return *this;
	}

	Vect& to_int (  )
	{
		x = int(x);
		y = int(y);
		return *this;
	}

	Vect& zero (  )
	{
		x = y = 0.0f;
		return *this;
	}

	Vect& set ( float _x, float _y )
	{
		x = _x;
		y = _y;
		return *this;
	}
	
	Vect& set ( Vect v )
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	float distance ( const Vect & b )
	{
		return sqrt((b.x - this->x) * (b.x - this->x) + (b.y - this->y) * (b.y - this->y));
	}

	static Vect abs (const Vect & v )
	{
		return Vect(std::fabs(v.x), std::fabs(v.y));
	}

	Vect abs (  )
	{
		this->x = std::fabs(this->x);
		this->y = std::fabs(this->y);

		return *this;
	}

	bool is_zero (  )
	{
		return x == 0 && y == 0;
	}

	Vect & negative (  )
	{
		if (this->x > 0)
			this->x = -this->x;
		if (this->y > 0)
			this->y = -this->y;

		return *this;
	}

	void print()
	{
		printf("vect{x=%f, y=%f}\n",this->x,this->y);
	}

	std::string toString (  )
	{
		return "x="+std::to_string(this->x)+",y="+std::to_string(this->y);
	}
};

#endif
