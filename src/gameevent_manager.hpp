#ifndef GAMEEVENT_HPP
#define GAMEEVENT_HPP

#include "gameentity.hpp"

struct SGameEvent
{
	CGameEntity * receiver;
	string event;
	
	SGameEvent ( CGameEntity * r=0, string e="" )
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
		vector <SGameEvent> events;
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
		~CGameEventManager (  )
		{
			if (singleton)
				delete singleton;
		}
		
		static CGameEventManager * instance (  )
		{
			if (!singleton)
				singleton = new CGameEventManager();
			
			return singleton;
		}
		
		void add_event ( SGameEvent e )
		{
			events.push_back(e);
			count = 0; // resta count para evitar perder processamento dos eventos
			set_state(1);
		}
		
		vector <SGameEvent> get_events ( CGameEntity * who )
		{
			vector <SGameEvent> aux;
			
			for (vector <SGameEvent>::iterator i = events.begin(); i != events.end(); i++)
				if ((*i).receiver == who && (*i).receiver != 0)
					aux.push_back((*i));

			return aux;
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 0:
					if (events.size() > 0)
						set_state(1);
					break;
				
				case 1:
					if (count++ == max)
					{
						count = 0;
						events.clear();
						set_state(0);
					}
					break;
				
				default:
					set_state(0);
					break;
			}
		
			return get_state();
		}
};

CGameEventManager * CGameEventManager::singleton = 0;

#endif


