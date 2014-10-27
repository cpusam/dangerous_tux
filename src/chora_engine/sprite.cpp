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
SDL_Rect SCollisionRect::get_world_rect ( SVect p )
{
	return (SDL_Rect){p.x + x, p.y + y, w, h};
}

//////////////////////////////////////////////////////////
void CCollisionFrame::add_rect ( SCollisionRect r )
{
	rects.push_back(r);
}

bool CCollisionFrame::set_rect ( int i, SCollisionRect r )
{
	if (i > -1 && i < rects.size())
	{
		rects[i] = r;
		return true;
	}
	
	return false;
}

void CCollisionFrame::clear_rects (  )
{
	source = (SDL_Rect){0,0,0,0};
	rects.clear();
}

std::vector <SCollisionRect> CCollisionFrame::get_rects (  )
{
	return rects;
}

SCollisionRect CCollisionFrame::get_rect ( int i )
{
	if (i > -1 && i < rects.size())
		return rects[i];
	
	return SCollisionRect();
}

std::vector <SCollisionRect> CCollisionFrame::get_rects_type ( int t )
{
	std::vector <SCollisionRect> cr;
	
	for (int i(0); i < rects.size(); i++)
		if (t == rects[i].get_type())
			cr.push_back(rects[i]);
	
	return cr;
}

//////////////////////////////////////////////////////////////
#ifndef USE_SDL2
	void CSprite::set_coll_frames ( SDL_Surface * s, std::vector <CCollisionFrame> c_f )
#else
	void CSprite::set_coll_frames ( SDL_Texture * t, std::vector <CCollisionFrame> c_f )
#endif
{
	coll_frames = c_f;
	
	for (int i = 0; i < c_f.size(); i++)
	#ifndef USE_SDL2
		CAnimation::add_frame(s, c_f[i]);
	#else
		CAnimation::add_frame(t, c_f[i]);
	#endif
}
	
#ifndef USE_SDL2
	void CSprite::add_frame ( SDL_Surface * s, CCollisionFrame c )
#else
	void CSprite::add_frame ( SDL_Texture * t, CCollisionFrame c )
#endif
{
	coll_frames.push_back(c);
	#ifndef USE_SDL2
		CAnimation::add_frame(s, static_cast<CAnimationFrame>(c));
	#else
		CAnimation::add_frame(t, static_cast<CAnimationFrame>(c));
	#endif
}

CCollisionFrame CSprite::get_coll_frame ( int i )
{
	return coll_frames.at(i);
}

std::vector <CCollisionFrame> CSprite::get_coll_frames (  )
{
	return coll_frames;
}





