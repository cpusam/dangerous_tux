#ifndef GAMESIGNAL_HPP
#define GAMESIGNAL_HPP

#include "gameentity.hpp"
#include "chora_engine/collision.hpp"
#include "chora_engine/animation.hpp"
#include "chora_engine/util.hpp"

class CKernelSignal: public CGameEntity
{
	protected:
		CAnimation anim;
		CCamera * cam;
		SVect kernel_pos;
		bool visible;
	
	public:
		#ifndef USE_SDL2
			CKernelSignal (  );
		#else
			CKernelSignal ( SDL_Renderer * r );
		#endif
		
		virtual ~CKernelSignal (  );
			
		void set_cam ( CCamera * c );
		
		void set_kernel_pos ( SVect kp );
		
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
		CAnimation anim;
		CCamera * cam;
		SVect exit_pos;
		bool visible;
	
	public:
		#ifndef USE_SDL2
			CExitSignal (  );
		#else
			CExitSignal ( SDL_Renderer * r );
		#endif
		
		virtual ~CExitSignal (  );
		
		void set_cam ( CCamera * c );
		
		void set_exit_pos ( SVect ep );
		
		void show ( bool s=true );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
		int update (  );
};

#endif
