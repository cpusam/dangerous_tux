#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

<<<<<<< HEAD
#include <string>
#include <vector>

#include "chora_engine/chora.hpp"
#include "chora_engine/statemachine.hpp"
#include "chora_engine/movable.hpp"
#include "chora_engine/camera.hpp"
#include "gameevent_manager.hpp"

=======
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
struct SGameEvent;

class CGameEntity: public CStateMachine, public CMovable
{
	protected:
<<<<<<< HEAD
		std::string id;
		std::vector <SGameEvent> events; // eventos recebidos de outras entidades
	
	public:
		CGameEntity (  );
		virtual ~CGameEntity (  );
		
		void set_id ( std::string i );
		
		std::string get_id (  );
		
		protected:
			// método usado para processar eventos vindo de outras entidades
			virtual void process (  );
		
		public:
			virtual void input ( SDL_Event & event );
		
			virtual int update (  );
		
			#ifndef USE_SDL2
				virtual void draw ( CCamera * cam, SDL_Surface * screen );
			#else
				virtual void draw ( CCamera * cam, SDL_Renderer * renderer );
			#endif
=======
		string id;
		vector <SGameEvent> events; // eventos recebidos de outras entidades
	
	public:
		CGameEntity (  )
		{
		}
		
		void set_id ( string i )
		{
			id = i;
		}
		
		string get_id (  )
		{
			return id;
		}
		
		protected:
			// método usado para processar eventos vindo de outras entidades
			virtual void process (  )
			{
			}
		
		public:
			virtual void input ( SDL_Event & event )
			{
			}
		
			virtual int update (  )
			{
				return get_state();
			}
		
			#ifndef USE_SDL2
				virtual void draw ( CCamera * cam, SDL_Surface * screen )
			#else
				virtual void draw ( CCamera * cam, SDL_Renderer * renderer )
			#endif
			{
			}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

#endif

