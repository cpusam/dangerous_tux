/*
 Copyright (C) 2014 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#ifndef CHORA_STATEMACHINE_HPP
#define CHORA_STATEMACHINE_HPP

#include <vector>
#include "sdl.hpp"

#define DEFAULT_STATE 0
#define DEFAULT_STATE_ID -9999

class StateMachine
{
	private:
		int state;

	public:
		StateMachine (  )
		{
			state = DEFAULT_STATE;
		}

		virtual ~StateMachine (  )
		{
		}

		virtual void set_state ( int s );
		virtual int get_state (  );
		virtual void input ( SDL_Event & event );
		virtual int update (  );
};

class State
{
	private:
		int id;

	public:
		State (  )
		{
			id = DEFAULT_STATE_ID;
		}

		void set_id ( int i )
		{
			id = i;
		}

		int get_id (  )
		{
			return id;
		}
};

class StatePatternMachine
{
	private:
		State * curr_state;
		std::vector <State *> state;

	public:
		StatePatternMachine (  )
		{
			curr_state = 0; // default
		}

		virtual ~StatePatternMachine (  )
		{
		}

		virtual bool has_state ( State * s );
		virtual bool add_state ( State * s );
		virtual bool set_state ( int id ); // troca de estado
		virtual State * get_state ( int id );
		virtual State * get_curr_state (  );
		virtual int update (  ); // retorna o id do estado
};

#endif


