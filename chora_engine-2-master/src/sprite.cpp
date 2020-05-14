#include "sprite.hpp"

SCollisionRect::SCollisionRect (  )
{
	type = UNDEFINED_RECT;
	x = y = 0;
	w = h = 0;
}

SCollisionRect::SCollisionRect ( int t, SDL_Rect r )
{
	set_type(t);
	x = r.x;
	y = r.y;
	w = r.w;
	h = r.h;
}

void SCollisionRect::set_rect ( int t, SDL_Rect r )
{
	set_type(t);
	x = r.x;
	y = r.y;
	w = r.w;
	h = r.h;
}

void SCollisionRect::set_type ( int t )
{
	if (t < UNDEFINED_RECT)
		t = UNDEFINED_RECT;

	type = t;
}

int SCollisionRect::get_type (  )
{
	return type;
}

// pega o retangulo em releção a posição p no mundo
SDL_Rect SCollisionRect::get_world_rect ( Vect p )
{
	return (SDL_Rect){int(p.x + x), int(p.y + y), w, h};
}

//////////////////////////////////////////////////////////
void CollisionFrame::set_source ( SDL_Rect src )
{
	AnimationFrame::set_source(src);
	src.x = src.y = 0;
	AnimationFrame::set_destiny(src);
}

void CollisionFrame::add_rect ( SCollisionRect r )
{
	rects.push_back(r);
}

bool CollisionFrame::set_rect ( int i, SCollisionRect r )
{
	if (i > -1 && i < (int)rects.size())
	{
		rects[i] = r;
		return true;
	}

	return false;
}

void CollisionFrame::clear_rects (  )
{
	source = (SDL_Rect)
	{
		0,0,0,0
	};
	rects.clear();
}

std::vector <SCollisionRect> CollisionFrame::get_rects (  )
{
	return rects;
}

SCollisionRect CollisionFrame::get_rect ( int i )
{
	if (i > -1 && i < (int)rects.size())
		return rects[i];

	return SCollisionRect();
}

std::vector <SCollisionRect> CollisionFrame::get_rects_type ( int t )
{
	std::vector <SCollisionRect> cr;

	for (unsigned int i(0); i < rects.size(); i++)
		if (t == rects[i].get_type())
			cr.push_back(rects[i]);

	return cr;
}

//////////////////////////////////////////////////////////////
void Sprite::set_coll_frames ( SDL_Texture * t, std::vector <CollisionFrame> c_f )
{
	coll_frames = c_f;

	for (unsigned int i = 0; i < c_f.size(); i++)
		Animation::add_frame(t, c_f[i]);
}

void Sprite::add_frame ( SDL_Texture * t, CollisionFrame c )
{
	coll_frames.push_back(c);
	Animation::add_frame(t, c);
}

CollisionFrame Sprite::get_coll_frame ( int i )
{
	return coll_frames.at(i);
}

std::vector <CollisionFrame> Sprite::get_coll_frames (  )
{
	return coll_frames;
}





