#ifndef GAMESIGNAL_HPP
#define GAMESIGNAL_HPP

#include "gameentity.hpp"
#include "collision.hpp"
#include "animation.hpp"
#include "util.hpp"

class CKernelSignal: public CGameEntity
{
	protected:
		Animation anim;
		Camera * cam;
		Vect kernel_pos;
		bool visible;
	
	public:
		CKernelSignal ( SDL_Renderer * r );

		virtual ~CKernelSignal (  );
			
		void set_cam ( Camera * c );
		
		void set_kernel_pos ( Vect kp );
		
		void show ( bool s=true );

		void draw ( SDL_Renderer * renderer );
		
		int update (  );
};

class CExitSignal: public CGameEntity
{
	protected:
		Animation anim;
		Camera * cam;
		Vect exit_pos;
		bool visible;
	
	public:
		CExitSignal ( SDL_Renderer * r );
		
		virtual ~CExitSignal (  );
		
		void set_cam ( Camera * c );
		
		void set_exit_pos ( Vect ep );
		
		void show ( bool s=true );

		void draw ( SDL_Renderer * renderer );

		int update (  );
};

#endif
