#ifndef GAMEEVENT_HPP
#define GAMEEVENT_HPP

#include "gameentity.hpp"

<<<<<<< HEAD
class CGameEntity;

struct SGameEvent
{
	CGameEntity * receiver;
	std::string event;
	
	SGameEvent ( CGameEntity * r=0, std::string e="" )
=======
struct SGameEvent
{
	CGameEntity * receiver;
	string event;
	
	SGameEvent ( CGameEntity * r=0, string e="" )
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
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
<<<<<<< HEAD
		std::vector <SGameEvent> events;
=======
		vector <SGameEvent> events;
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
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
<<<<<<< HEAD
		virtual ~CGameEventManager (  );
		
		static CGameEventManager * instance (  );
		
		void add_event ( SGameEvent e );
		
		std::vector <SGameEvent> get_events ( CGameEntity * who );
		
		int update (  );
};

=======
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

>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
#endif


