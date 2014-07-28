#include "movable.hpp"

void CMovable::set_pos ( SVect p )
{
	pos = p;
}

SVect CMovable::get_pos (  )
{
	return pos;
}

void CMovable::set_vel ( SVect v )
{
	vel = v;
}

SVect CMovable::get_vel (  )
{
	return vel;
}

void CMovable::set_acc ( SVect a )
{
	acc = a;
}

SVect CMovable::get_acc (  )
{
	return acc;
}

void CMovable::set_dim ( SDL_Rect d )
{
	dim = d;
}

SDL_Rect CMovable::get_dim (  )
{
	return dim;
}


