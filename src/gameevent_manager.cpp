#include "gameevent_manager.hpp"

CGameEventManager * CGameEventManager::singleton = 0;

CGameEventManager::~CGameEventManager (  )
{
	if (singleton)
		delete singleton;
}

CGameEventManager * CGameEventManager::instance (  )
{
	if (!singleton)
		singleton = new CGameEventManager();
	
	return singleton;
}

void CGameEventManager::add_event ( SGameEvent e )
{
	events.push_back(e);
	count = 0; // resta count para evitar perder processamento dos eventos
	set_state(1);
}

std::vector <SGameEvent> CGameEventManager::get_events ( CGameEntity * who )
{
	std::vector <SGameEvent> aux;
	
	for (std::vector <SGameEvent>::iterator i = events.begin(); i != events.end(); i++)
		if ((*i).receiver == who && (*i).receiver != 0)
			aux.push_back((*i));

	return aux;
}

int CGameEventManager::update (  )
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



