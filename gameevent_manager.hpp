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

		CGameEventManager (  )
		{
			max = 2;
			count = 0;
			set_state(1);
		}

	public:
		static CGameEventManager * instance (  )
		{
			if (!singleton)
				singleton = new CGameEventManager();
			
			return singleton;
		}
		
		bool add_event ( SGameEvent e )
		{
			if (get_state() == 1)
			{
				events.push_back(e);
				return true;
			}
			
			return false;
		}
		
		vector <SGameEvent> get_events ( CGameEntity * who )
		{
			vector <SGameEvent> aux;
			
			for (vector <SGameEvent>::iterator i = events.begin(); i != events.end(); i++)
			{
				if ((*i).receiver == who && (*i).receiver != 0)
					aux.push_back((*i));
			}
			
			return aux;
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 0:
					break;
				
				case 1:
					if (count++ == max)
					{
						count = 0;
						events.clear();
					}
					break;
				
				default:
					break;
			}
		
			return get_state();
		}
};

CGameEventManager * CGameEventManager::singleton = 0;

#endif


