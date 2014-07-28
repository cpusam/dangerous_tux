
#include "camera.hpp"


void CCamera::lookat ( SVect p )
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

SVect CCamera::get_position (  )
{
	return position;
}

SVect CCamera::get_focus (  )
{
	return focus;
}

void CCamera::set_focus ( SVect f )
{
	if (f.x < 0 || f.y < 0)
		return;
	
	if (f.x > dimension.w || f.y > dimension.h)
		return;
	
	focus = f;
}

SDL_Rect CCamera::get_view (  )
{
	return (SDL_Rect){int(position.x), int(position.y), dimension.w,dimension.h};
}

void CCamera::set_position ( SVect p )
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

SDL_Rect CCamera::get_dimension (  )
{
	return dimension;
}

SDL_Rect CCamera::get_limit (  )
{
	return limit;
}

void CCamera::set_limit ( SDL_Rect l )
{
	limit = l;
}

