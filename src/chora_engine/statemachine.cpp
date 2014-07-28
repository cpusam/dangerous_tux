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

