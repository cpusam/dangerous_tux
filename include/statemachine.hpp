#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

class CStateMachine
{
	private:
		int state;
		static int i;	
	public:
		CStateMachine (  )
		{
			state = 0; // default
		}
		void set_state ( int s ){ state = s;}
		int get_state (  ){ return state;}
		virtual int update (  ) = 0;
};

#endif


