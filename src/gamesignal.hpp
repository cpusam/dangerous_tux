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
		#ifndef USE_SDL2
			CKernelSignal (  );
		#else
			CKernelSignal ( SDL_Renderer * r );
		#endif
		
		virtual ~CKernelSignal (  );
			
		void set_cam ( Camera * c );
		
		void set_kernel_pos ( Vect kp );
		
		void show ( bool s=true );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
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
		#ifndef USE_SDL2
			CExitSignal (  );
		#else
			CExitSignal ( SDL_Renderer * r );
		#endif
		
		virtual ~CExitSignal (  );
		
		void set_cam ( Camera * c );
		
		void set_exit_pos ( Vect ep );
		
		void show ( bool s=true );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
		int update (  );
};

#endif
