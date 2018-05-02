#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include "Chora.hpp"
#include "vect.hpp"
#include "util.hpp"
#include "camera.hpp"
#include "animation.hpp"


class CGameOver: public StateMachine
{
	protected:
		SDL_Texture * background;
		SDL_Texture * title;
		Animation stars;
		
		Vect t_pos;
		Vect s_pos;
	
	public:
		CGameOver ( SDL_Renderer * r );

		virtual ~CGameOver (  );
		
		void reset (  );
		
		int update (  );
		
		void draw ( Camera * cam, SDL_Renderer * renderer );
};

#endif
