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

#define DEFAULT_STATE 0
#define DEFAULT_STATE_ID -9999

class CStateMachine
{
	private:
		int state;

	public:
		CStateMachine (  )
		{
			state = DEFAULT_STATE;
		}
	
		virtual ~CStateMachine (  )
		{
		}

		virtual void set_state ( int s );
		virtual int get_state (  );
		virtual int update (  );
};

class CState
{
	private:
		int id;
	
	public:
		CState (  )
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

class CStatePatternMachine
{
	private:
		CState * curr_state;
		std::vector <CState *> state;
	
	public:
		CStatePatternMachine (  )
		{
			curr_state = 0; // default
		}
	
		virtual ~CStatePatternMachine (  )
		{
		}
		
		virtual bool have_state ( CState * s );
		virtual bool add_state ( CState * s );
		virtual bool set_state ( int id ); // troca de estado
		virtual CState * get_state ( int id );
		virtual CState * get_curr_state (  );
		virtual int update (  ); // retorna o id do estado
};

#endif


