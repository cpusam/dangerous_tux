
#ifndef JETPACK_HPP
#define JETPACK_HPP

class CJetpack: public CStateMachine
{
	protected:
		float fuel;
		float fuel_max;
		float spend;
		bool has; // se tem ou não o jetpack

	public:
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
};

#endif

