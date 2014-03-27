#ifndef ALIENS_HPP
#define ALIENS_HPP

#include "gameentity.hpp"
#include "player.hpp"

enum EAlienState
{
	STOPED_ALIEN,
	MOVING_ALIEN,
	SHOOTING_ALIEN,
	DYING_ALIEN,
	INACTIVE_ALIEN
};

enum EAlienDirection
{
	LEFT_ALIEN,
	RIGHT_ALIEN,
	DOWN_ALIEN,
	UP_ALIEN
};

struct SAlienConfig
{
	SVect vel_max;
	SVect circle_vel_max;
	SVect acc;
	SVect circle_acc;
	float shot_vel;
	
	SAlienConfig (  )
	{
		vel_max.x = 3;
		vel_max.y = 14;
		acc.x = 1;
		acc.y = 1;
		shot_vel = 10;
		
		circle_vel_max.x = 3;
		circle_vel_max.y = 3;
		circle_acc.x = 1;
		circle_acc.y = 1;
	}
};

#endif

