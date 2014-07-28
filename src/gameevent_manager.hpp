#ifndef GAMEEVENT_HPP
#define GAMEEVENT_HPP

#include "gameentity.hpp"

class CGameEntity;

struct SGameEvent
{
	CGameEntity * receiver;
	std::string event;
	
	SGameEvent ( CGameEntity * r=0, std::string e="" )
	{
		receiver = r;
		event = e;
	}
};

// Essa classe deve ser a última a ser atualizada
class CGameEventManager: public CStateMachine
{
	private:
		static CGameEventManager * singleton;
		std::vector <SGameEvent> events;
		int count;
		int max;

	private:
		CGameEventManager (  )
		{
			max = 2;
			count = 0;
			set_state(1);
		}

	public:
		virtual ~CGameEventManager (  );
		
		static CGameEventManager * instance (  );
		
		void add_event ( SGameEvent e );
		
		std::vector <SGameEvent> get_events ( CGameEntity * who );
		
		int update (  );
};

#endif


