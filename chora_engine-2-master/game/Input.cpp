#include "Input.hpp"

Input::Input()
{
	in = -1;
	press = false;
	set_state(State::FREE);
}

Input::Input ( int k )
{
	pressTime = 0;
	in = k;
	press = false;
	set_state(State::FREE);
}

Input::~Input()
{
	//dtor
}

Uint32 Input::getPressTime (  )
{
	return pressTime;
}

bool Input::isPressed()
{
	return press;
}

void Input::setInput ( int k )
{
	in = k;
}

int Input::getInput (  )
{
	return in;
}

void Input::reset (  )
{
	press = false;
	set_state(State::FREE);
}

void Input::input ( SDL_Event & event )
{
	
}

int Input::update (  )
{
	switch (get_state())
	{
		case State::FREE:
			if (press)
			{
				pressTime = SDL_GetTicks();
				set_state(State::PRESS);
			}
			break;
		
		case State::PRESS:
			if (press)
			{
				set_state(State::HOLD);
			}
			else
			{
				set_state(State::RELEASE);
			}
			break;
		
		case State::HOLD:
			if (press == false)
				set_state(State::RELEASE);
			break;
		
		case State::RELEASE:
			press = false;
			set_state(State::FREE);
			break;
		
		default:
			set_state(State::FREE);
			break;
	}
	
	return get_state();
}

