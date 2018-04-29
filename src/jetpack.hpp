
#ifndef JETPACK_HPP
#define JETPACK_HPP

#include "statemachine.hpp"

class CJetpack: public StateMachine
{
	protected:
		float fuel;
		float fuel_max;
		float spend;
		bool has; // se tem ou não o jetpack

	public:
		CJetpack ( float f_max, float s, bool h );
		virtual ~CJetpack (  );
		
		void reload (  );
		
		float get_fuel (  );
		
		void set_jetpack ( bool h );
		
		bool has_fuel (  );
		
		bool has_jetpack (  );
		
		int update (  );
};

#endif

