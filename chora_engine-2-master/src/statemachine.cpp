#include "statemachine.hpp"

void StateMachine::set_state ( int s )
{
	state = s;
}

int StateMachine::get_state (  )
{
	return state;
}

void StateMachine::input ( SDL_Event & event )
{
}

int StateMachine::update (  )
{
	return get_state();
}


//////////////////////////////////////////////////////////////////
bool StatePatternMachine::has_state ( State * s )
{
	for (unsigned int i = 0; i < state.size(); i++)
		if (state[i] == s)
			return true;

	return false;
}

bool StatePatternMachine::add_state ( State * s )
{
	if (!has_state(s))
	{
		state.push_back(s);
		return true;
	}

	return false;
}

bool StatePatternMachine::set_state ( int id )
{
	for (unsigned int i = 0; i < state.size(); i++)
		if (state[i]->get_id() == id)
		{
			curr_state = state[i];
			return true;
		}

	return false;
}

State * StatePatternMachine::get_state ( int id )
{
	for (unsigned int i = 0; i < state.size(); i++)
		if (state[i]->get_id() == id)
			return state[i];

	return 0;
}

State * StatePatternMachine::get_curr_state (  )
{
	return curr_state;
}

int StatePatternMachine::update (  )   // retorna o id do estado
{
	return DEFAULT_STATE_ID;
}
