#include "gameentity.hpp"

CGameEntity::CGameEntity (  )
{
}

CGameEntity::~CGameEntity (  )
{
}

void CGameEntity::set_id ( std::string i )
{
	id = i;
}

std::string CGameEntity::get_id (  )
{
	return id;
}

void CGameEntity::process (  )
{
}

void CGameEntity::input ( SDL_Event & event )
{
}

int CGameEntity::update (  )
{
	return get_state();
}

#ifndef USE_SDL2
	 void CGameEntity::draw ( Camera * cam, SDL_Surface * screen )
#else
	 void CGameEntity::draw ( Camera * cam, SDL_Renderer * renderer )
#endif
{
}

