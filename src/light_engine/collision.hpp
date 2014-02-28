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


#endif


