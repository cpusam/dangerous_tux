
#ifndef JETPACK_HPP
#define JETPACK_HPP

<<<<<<< HEAD
#include "chora_engine/statemachine.hpp"

=======
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
class CJetpack: public CStateMachine
{
	protected:
		float fuel;
		float fuel_max;
		float spend;
		bool has; // se tem ou não o jetpack

	public:
<<<<<<< HEAD
		CJetpack ( float f_max, float s, bool h );
		virtual ~CJetpack (  );
		
		void reload (  );
		
		float get_fuel (  );
		
		void set_jetpack ( bool h );
		
		bool has_fuel (  );
		
		bool has_jetpack (  );
		
		int update (  );
=======
		CJetpack ( float f_max, float s, bool h )
		{
			fuel = fuel_max = f_max;
			spend = s;
			has = h;
			set_state(1);
		}
		
		void reload (  )
		{
			fuel = fuel_max;
		}
		
		float get_fuel (  )
		{
			return fuel;
		}
		
		void set_jetpack ( bool h )
		{
			has = h;
			if (h)
			{
				fuel = fuel_max;
			}
		}
		
		bool has_fuel (  )
		{
			return (fuel > 0);
		}
		
		bool has_jetpack (  )
		{
			return has;
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 0:
					if (has)
						set_state(1);
					break;

				case 1: // em uso
					if (!has)
					{
						set_state(0);
						break;
					}
					
					fuel -= spend;
					if (fuel < 0)
						fuel = 0;
					break;
			}
			
			return get_state();
		}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

#endif

