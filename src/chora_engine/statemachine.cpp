#include "statemachine.hpp"

void CStateMachine::set_state ( int s )
{
	state = s;
}

int CStateMachine::get_state (  )
{
	return state;
}

int CStateMachine::update (  )
{
	return get_state();
}


//////////////////////////////////////////////////////////////////
bool CStatePatternMachine::have_state ( CState * s )
{
	for (int i = 0; i < state.size(); i++)
		if (state[i] == s)
			return true;
	
	return false;
}

bool CStatePatternMachine::add_state ( CState * s )
{
	if (!have_state(s))
	{
		state.push_back(s);
		return true;
	}
	
	return false;
}

bool CStatePatternMachine::set_state ( int id )
{
	for (int i = 0; i < state.size(); i++)
		if (state[i]->get_id() == id)
		{
			curr_state = state[i];
			return true;
		}
	
	return false;
}

CState * CStatePatternMachine::get_state ( int id )
{
	for (int i = 0; i < state.size(); i++)
		if (state[i]->get_id() == id)
			return state[i];

	return 0;
}

CState * CStatePatternMachine::get_curr_state (  )
{
	return curr_state;
}

int CStatePatternMachine::update (  ) // retorna o id do estado
{
	return DEFAULT_STATE_ID;
}
