#include "flyeralien.hpp"

CFlyerAlien::CFlyerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, Vect i_p )
{
	SDL_Texture * texture = 0;
	SDL_Surface * aux = 0;
	dir = LEFT_ALIEN; // direção a seguir, LEFT_ALIEN ou RIGHT_ALIEN
	shot_dir = LEFT_ALIEN;
	move_distance = m_d;
	player = p;
	pos = init_pos = i_p;
	set_map(p->get_map());
	c_point.set(24,24);
	dim = (SDL_Rect){15,0,18,44};
	set_id("flyer_alien");
	gun.set_gun(true);
	gun.shot.set_id("alien_shot");
	gun.shot.add_target(player);
	
	anim.resize(3);
	
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path, "%s\\images\\flyeralien.png", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\images\\flyeralien.png", PREFIX);
		#endif
	#else
		char path[1024];
		#ifndef PREFIX
			sprintf(path, "./images/flyeralien.png");
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/flyeralien.png", PREFIX);
		#endif
	#endif

	texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CFlyerAlien: não conseguiu abrir flyeralien.png\n";

	// animações
	// voando
	anim[0].add_frame(texture, (SDL_Rect){0,0,48,48}, 3);
	// atirando
	anim[1].add_frame(texture, (SDL_Rect){0,0,48,48}, 3);
	// morrendo
	anim[2].add_frame(texture, (SDL_Rect){0,48*2,48,48}, 6);
	anim[2].add_frame(texture, (SDL_Rect){48,48*2,48,48}, 6);
	anim[2].add_frame(texture, (SDL_Rect){48*2,48*2,48,48}, 6);
	
	reset();
}

CFlyerAlien::~CFlyerAlien (  )
{
	anim[0].destroy_textures();
}


void CFlyerAlien::set_map ( TileMap * m )
{
	map = m;
	gun.shot.set_map(m);
}

void CFlyerAlien::reset (  )
{
	count = -1.57;
	step = 0.05;
	pos.x = init_pos.x + move_distance * cos(count);
	pos.y = init_pos.y + move_distance * sin(count);
	curr_anim = &anim[0]; // alien parado
	set_state(STOPED_ALIEN);
}

void CFlyerAlien::kill (  )
{
	acc.zero();
	vel.zero();
	curr_anim = &anim[2]; // morrendo
	curr_anim->reset();
	set_state(DYING_ALIEN);
}

void CFlyerAlien::draw ( Camera * cam, SDL_Renderer * renderer )
{
	if (curr_anim)
		curr_anim->draw(renderer, cam, pos.x, pos.y);

	gun.draw(cam, renderer);

	if (get_state() == INACTIVE_ALIEN)
		return;
	
	if (get_state() == STOPED_ALIEN)
	{
		if (init_pos.x + c_point.x > player->get_pos().x)
			dir = LEFT_ALIEN; // move para esquerda
		else
			dir = RIGHT_ALIEN; // move para direita
		
		//count = 0;
		curr_anim = &anim[0]; // voando
		curr_anim->reset();
		set_state(MOVING_ALIEN);
	}
	else
	{
		SDL_Rect a, b, c;
	
		a = cam->get_dimension();
		a.x = cam->get_position().x - map->get_tilesize();
		a.y = cam->get_position().y;
		a.w += map->get_tilesize() * 2;
	
		b = dim;
		b.x = pos.x + dim.x;
		b.y = pos.y + dim.y;

		c.x = init_pos.x + c_point.x;
		c.y = init_pos.y + c_point.y;
		c.w = dim.w;
		c.h = dim.h;
	
		if (!boundingbox(a, b) && !boundingbox(c, a))
		{
			reset();
		}
	}
}

void CFlyerAlien::process (  )
{
	shot_at = touched_player = false;
	events = CGameEventManager::instance()->get_events(static_cast<CGameEntity *>(this));
	for (vector <SGameEvent>::iterator it = events.begin(); it != events.end(); it++)
	{
		if ((*it).event == "shot_at")
		{
			shot_at = true;
			break;
		}
	}
}

int CFlyerAlien::update (  )
{
	switch (get_state())
	{
		case STOPED_ALIEN:
			gun.update();
			break;
	
		case MOVING_ALIEN:
			if (dir == RIGHT_ALIEN)
			{
				count += step;
				if (count > 3.14 * 2.0)
					count = 0;
			}
			else
			{
				count -= step;
				if (count < -(3.14 * 2.0))
					count = 0;
			}

			pos.x = init_pos.x + move_distance * cos(count);
			pos.y = init_pos.y + move_distance * sin(count);

			if (player->get_pos().x < pos.x + c_point.x)
				shot_dir = LEFT_ALIEN;
			else
				shot_dir = RIGHT_ALIEN;
			
			if (!gun.was_used())
			{
				int r = (75.0 * rand()) / (RAND_MAX + 1.0);
				if (r == 0)
					gun.fire(pos, shot_dir, Vect((shot_dir == LEFT_ALIEN)? -config.shot_vel: config.shot_vel, 0));
				
				if (gun.was_used())
				{
					acc.zero();
					vel.zero();

					curr_anim = &anim[1]; // atirando							
					curr_anim->reset();
					set_state(SHOOTING_ALIEN);
					break;
				}
			}
			
			process();
			
			if (shot_at || touched_player)
			{
				kill();
				break;
			}
			
			curr_anim->update();
			gun.update();
			break;
		
		case SHOOTING_ALIEN:
			gun.update();
			
			process();
			
			if (shot_at || touched_player)
			{
				kill();
				break;
			}
			
			if (curr_anim->update() == 3)
			{
				curr_anim = &anim[0]; // movendo/voando
				curr_anim->reset();
				set_state(MOVING_ALIEN);
			}
			break;
		
		case DYING_ALIEN:
			gun.update();
			
			if (curr_anim->update() == 3)
			{
				curr_anim = 0;
				set_state(INACTIVE_ALIEN);
			}
			break;
		
		case INACTIVE_ALIEN:
			gun.update();
			break;
		
		default:
			set_state(STOPED_ALIEN);
			break;
	}

	return get_state();
}



