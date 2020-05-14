#include "Key.hpp"

Key::Key(): Input(-1)
{
	key = 0;
	press = false;
	set_state(FREE);
}

Key::Key ( SDL_Keycode k ): Input(static_cast<int>(k))
{
	key = k;
}

Key::~Key()
{
	//dtor
}

void Key::setKey ( SDL_Keycode k )
{
	key = k;
	Input::setInput(static_cast<int>(k));
}

SDL_Keycode Key::getKey (  )
{
	return key;
}

void Key::input ( SDL_Event & event )
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == key)
		{
			press = true;
		}
	}
	
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == key)
			press = false;
	}
}

