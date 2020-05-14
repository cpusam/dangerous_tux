#include "movable.hpp"
#include "SDL_gfx/SDL_framerate.hpp"

void Movable::set_pos ( Vect p )
{
	pos = p;
}

Vect Movable::get_pos (  )
{
	return pos;
}

void Movable::setPosition ( Vect p )
{
	pos = p;
}

Vect Movable::getPosition (  )
{
	return pos;
}

void Movable::set_vel ( Vect v )
{
	vel = v;
}

void Movable::set_vel_x ( float x )
{
	vel.x = x;
}

void Movable::set_vel_y ( float y )
{
	vel.y = y;
}

Vect Movable::get_vel (  )
{
	return vel;
}

float Movable::get_vel_x (  )
{
	return vel.x;
}

float Movable::get_vel_y (  )
{
	return vel.y;
}

void Movable::set_acc ( Vect a )
{
	acc = a;
}

void Movable::set_acc_x ( float x )
{
	acc.x = x;
}

void Movable::set_acc_y ( float y )
{
	acc.y = y;
}

Vect Movable::get_acc (  )
{
	return acc;
}

float Movable::get_acc_x (  )
{
	return acc.x;
}

float Movable::get_acc_y (  )
{
	return acc.y;
}

void Movable::set_dim ( SDL_Rect d )
{
	dim = d;
}

SDL_Rect Movable::get_dim (  )
{
	return dim;
}

