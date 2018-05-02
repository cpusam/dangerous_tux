#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <string>
#include <vector>

#include "Chora.hpp"
#include "statemachine.hpp"
#include "movable.hpp"
#include "camera.hpp"
#include "gameevent_manager.hpp"

struct SGameEvent;

class CGameEntity: public StateMachine, public Movable
{
	protected:
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

			virtual void draw ( Camera * cam, SDL_Renderer * renderer );
};

#endif

