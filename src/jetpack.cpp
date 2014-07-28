#include "jetpack.hpp"

CJetpack::CJetpack ( float f_max, float s, bool h )
{
	fuel = fuel_max = f_max;
	spend = s;
	has = h;
	set_state(1);
}

CJetpack::~CJetpack (  )
{
}

void CJetpack::reload (  )
{
	fuel = fuel_max;
}

float CJetpack::get_fuel (  )
{
	return fuel;
}

void CJetpack::set_jetpack ( bool h )
{
	has = h;
	if (h)
	{
		fuel = fuel_max;
	}
}

bool CJetpack::has_fuel (  )
{
	return (fuel > 0);
}

bool CJetpack::has_jetpack (  )
{
	return has;
}

int CJetpack::update (  )
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


