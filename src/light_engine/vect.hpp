#ifndef VECT_HPP
#define VECT_HPP

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
