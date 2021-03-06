#include "walkeralien.hpp"

CWalkerAlien::CWalkerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, Vect i_p )
{
	SDL_Texture * texture = 0;
	SDL_Surface * aux = 0;
	dir = LEFT_ALIEN;
	player = p;
	move_distance = m_d;
	pos = init_pos = i_p;
	acc = config.acc;
	set_map(player->get_map());
	dim = (SDL_Rect){15,6,16,37};
	set_id("walker_alien");
	gun.set_gun(true);
	gun.shot.set_id("alien_shot");
	gun.shot.add_target(p);
	Vect pd[2] = {Vect(0,20),Vect(38,20)};
	gun.set_pos_dir(pd);
	
	// tiles onde se pode caminhar
	coll_tiles.push_back('a');
	coll_tiles.push_back('b');
	coll_tiles.push_back('c');
	coll_tiles.push_back('s');
	coll_tiles.push_back('v');
	coll_tiles.push_back('u');
	coll_tiles.push_back('w');
	coll_tiles.push_back('Q');
	coll_tiles.push_back('R');
	
	// pontos de colisão
	// colisão a direita
	c_point.push_back(Vect(31,6));
	c_point.push_back(Vect(31,43));
	// colisão a esquerda
	c_point.push_back(Vect(15,6));
	c_point.push_back(Vect(15,43));
	// colisão abaixo
	c_point.push_back(Vect(15,43));
	c_point.push_back(Vect(23,43));
	c_point.push_back(Vect(31,43));
	// colisão acima
	c_point.push_back(Vect(15,6));
	c_point.push_back(Vect(31,6));
	// centro de colisão
	c_point.push_back(Vect(23,25));
	// meio da lateral esquerda
	c_point.push_back(Vect(14,25));
	// meio da lateral direita
	c_point.push_back(Vect(32,25));

	anim.resize(6);

	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path, "%s\\images\\walkeralien_right.png", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\images\\walkeralien_right.png", PREFIX);
		#endif
	#else
		char path[1024];
		#ifndef PREFIX
			sprintf(path, "./images/walkeralien_right.png");
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/walkeralien_right.png", PREFIX);
		#endif
	#endif
	
	texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CWalkerAlien: não foi possível abrir walkeralien_right.png\n";

	// virado para direita
	// caminhando para direita
	anim[0].add_frame(texture, (SDL_Rect){0,0,48,48}, 30);
	anim[0].add_frame(texture, (SDL_Rect){48,0,48,48}, 50);
	anim[0].add_frame(texture, (SDL_Rect){48*2,0,48,48}, 30);
	// atirando para direita
	anim[1].add_frame(texture, (SDL_Rect){0,48,48,48}, 30);
	anim[1].add_frame(texture, (SDL_Rect){48,48,48,48}, 30);
	anim[1].add_frame(texture, (SDL_Rect){48*2,48,48,48}, 30);
	// morrendo
	anim[4].add_frame(texture, (SDL_Rect){0,48*2,48,48}, 60);
	anim[4].add_frame(texture, (SDL_Rect){48,48*2,48,48}, 60);
	anim[4].add_frame(texture, (SDL_Rect){48*2,48*2,48,48}, 60);
	
	
	#if _WIN32 || _WIN64
		#ifndef PREFIX
			sprintf(path, "%s\\images\\walkeralien_left.png", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\images\\walkeralien_left.png", PREFIX);
		#endif
	#else
		#ifndef PREFIX
			sprintf(path, "./images/walkeralien_left.png");
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/walkeralien_left.png", PREFIX);
		#endif
	#endif
	
	texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CWalkerAlien: não foi possível abrir walkeralien_left.png\n";

	// virado para esquerda
	// caminhando para esquerda
	anim[2].add_frame(texture, (SDL_Rect){0,0,48,48}, 30);
	anim[2].add_frame(texture, (SDL_Rect){48,0,48,48}, 50);
	anim[2].add_frame(texture, (SDL_Rect){48*2,0,48,48}, 30);
	// atirano para esquerda
	anim[3].add_frame(texture, (SDL_Rect){0,48,48,48}, 30);
	anim[3].add_frame(texture, (SDL_Rect){48,48,48,48}, 30);
	anim[3].add_frame(texture, (SDL_Rect){48*2,48,48,48}, 30);
	//morrendo
	anim[5].add_frame(texture, (SDL_Rect){0,48*2,48,48}, 60);
	anim[5].add_frame(texture, (SDL_Rect){48,48*2,48,48}, 60);
	anim[5].add_frame(texture, (SDL_Rect){48*2,48*2,48,48}, 60);

	curr_anim = &anim[0];
	
	reset();
}

CWalkerAlien::~CWalkerAlien (  )
{
	anim[0].destroy_textures();
	anim[2].destroy_textures();
}

void CWalkerAlien::set_map ( TileMap * m )
{
	map = m;
	gun.shot.set_map(m);
}

void CWalkerAlien::reset (  )
{
	pos = init_pos;
	vel.y = 1;
	pos.y += c_point[5].y;
	collide_ver();
	set_state(STOPED_ALIEN);
}

int CWalkerAlien::collide_hor (  )
{
	int ret = 0;
	float p;

	if (vel.x == 0)
		return ret;
	
	for (int i = 0; i < 4; i++)
		if (has_coll_tile(map->get_tile(pos.x + c_point[i].x + vel.x, pos.y + c_point[i].y)))
		{
			if (i < 2)
			{
				// colisão à direita
				p = int(pos.x + c_point[i].x + vel.x) / map->get_tilesize();
				p *= map->get_tilesize();
				pos.x = p - c_point[i].x - 1;
				vel.x = 0;
				ret = 1;
			}
			else
			{
				// colisão à esquerda
				p = int(pos.x + c_point[i].x + vel.x) / map->get_tilesize();
				p += 1;
				p *= map->get_tilesize();
				pos.x = p - c_point[i].x;
				vel.x = 0;
				ret = -1;
			}
		}

	return 0;
}

int CWalkerAlien::collide_ver (  )
{
	int ret = 0;
	float p;
	
	if (vel.y == 0)
		return ret;

	for (int i = 4; i < 9; i++)
		if (has_coll_tile(map->get_tile(pos.x + c_point[i].x, pos.y + c_point[i].y + vel.y)))
		{
			if (i < 7)
			{
				// colisão em baixo
				p = int(pos.y + c_point[i].y + vel.y) / map->get_tilesize();
				p *= map->get_tilesize();
				pos.y = p - c_point[i].y - 1;
				vel.y = 0;
				ret = 1;
			}
			else
			{
				// colisão em cima
				p = int(pos.y + c_point[i].y + vel.y) / map->get_tilesize();
				p += 1;
				p *= map->get_tilesize();
				pos.y = p - c_point[i].y;
				vel.y = 0;
				ret = -1;
			}
		}
	
	return ret;
}

bool CWalkerAlien::has_coll_tile ( int tile )
{
	vector <int>::iterator it;
		for (it = coll_tiles.begin(); it != coll_tiles.end(); it++)
		{
			if (tile == *it)
			{
				return true;
			}
		}

	return false;
}

int CWalkerAlien::no_ground (  )
{
	if (!has_coll_tile(map->get_tile(pos.x + c_point[4].x, pos.y + c_point[4].y + 1)))
		return -1;
	
	if (!has_coll_tile(map->get_tile(pos.x + c_point[6].x, pos.y + c_point[6].y + 1)))
		return 1;

	return 0;
}

int CWalkerAlien::wall (  )
{
	if (has_coll_tile(map->get_tile(pos.x + c_point[10].x, pos.y + c_point[10].y))) // se colidiu a esquerda
		return -1;

	if (has_coll_tile(map->get_tile(pos.x + c_point[11].x, pos.y + c_point[11].y))) // se colidiu a direita
		return 1;

	return 0;
}

void CWalkerAlien::kill (  )
{
	acc.zero();
	vel.zero();
	
	if (dir == LEFT_ALIEN)
		curr_anim = &anim[5]; // morrendo para esquerda
	else
		curr_anim = &anim[4]; // morrendo para direita
	
	curr_anim->reset();
	set_state(DYING_ALIEN);
}

void CWalkerAlien::draw ( Camera * cam, SDL_Renderer * renderer )
{
	if (curr_anim)
		curr_anim->draw(renderer, cam, pos.x, pos.y);

	gun.draw(cam, renderer);

	
	if (get_state() == INACTIVE_ALIEN)
		return;
	
	if (get_state() == STOPED_ALIEN)
	{
		if (init_pos.x + c_point[9].x > player->get_pos().x)
		{
			dir = LEFT_ALIEN; // move para esquerda
			acc.x = -config.acc.x;
			curr_anim = &anim[2]; // caminhando para esquerda
		}
		else
		{
			dir = RIGHT_ALIEN; // move para direita
			acc.x = config.acc.x;
			curr_anim = &anim[0]; // caminhando para direita
		}

		vel.x = 0;
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

		c.x = init_pos.x + c_point[9].x;
		c.y = init_pos.y + c_point[9].y;
		c.w = dim.w;
		c.h = dim.h;
	
		if (!boundingbox(a, b) && !boundingbox(c, a))
		{
			reset();
		}
	}
}

void CWalkerAlien::process (  )
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

int CWalkerAlien::update (  )
{
	switch (get_state())
	{
		case STOPED_ALIEN:
			gun.update();
			break;
	
		case MOVING_ALIEN:
			acc.y = config.acc.y;
			
			if (dir == LEFT_ALIEN)
			{
				acc.x = -config.acc.x;
				curr_anim = &anim[2]; // caminhando para esquerda
				if (no_ground() == -1 || (pos.x + c_point[9].x) - init_pos.x < -move_distance || wall() == -1)
				{
					dir = RIGHT_ALIEN;
					acc.x = config.acc.x;
					vel.x = -vel.x;
					pos.x += vel.x;
					curr_anim = &anim[0];
					curr_anim->reset();
				}
			}
			else
			{
				acc.x = config.acc.x;
				curr_anim = &anim[0]; // caminhando para direita
				if (no_ground() == 1 || (pos.x + c_point[9].x) - init_pos.x > move_distance || wall() == 1)
				{
					dir = LEFT_ALIEN;
					acc.x = -config.acc.x;
					vel.x = -vel.x;
					pos.x += vel.x;
					curr_anim = &anim[2];
					curr_anim->reset();
				}
			}
			
			
			if (!gun.was_used())
			{
				int r = (75.0 * rand()) / (RAND_MAX + 1.0);
				if (r == 0)
					gun.fire(pos, dir, Vect((dir == LEFT_ALIEN)? -config.shot_vel: config.shot_vel, 0));
				
				if (gun.was_used())
				{
					acc.zero();
					vel.zero();
					if (dir == LEFT_ALIEN)
						curr_anim = &anim[3]; // atirando para esquerda
					else
						curr_anim = &anim[1]; // atirando para direita
					
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
			
			vel.x += acc.x;
			if (vel.x > config.vel_max.x)
				vel.x = config.vel_max.x;
			else if (vel.x < -config.vel_max.x)
				vel.x = -config.vel_max.x;

			collide_hor();
			pos.x += vel.x;

			vel.y += acc.y;
			if (vel.y > config.vel_max.y)
				vel.y = config.vel_max.y;
			else if (vel.y < -config.vel_max.y)
				vel.y = -config.vel_max.y;

			
			collide_ver();
			pos.y += vel.y;
			
			gun.update();
			curr_anim->update();
			break;
		
		case SHOOTING_ALIEN:
			if (curr_anim->update() == 3)
			{
				if (dir == LEFT_ALIEN)
				{
					acc.x = -config.acc.x;
					vel.x = 0;
					curr_anim = &anim[2]; // caminhando para esquerda
				}
				else
				{
					acc.x = config.acc.x;
					vel.x = 0;
					curr_anim = &anim[0]; // caminhando para direita
				}
				
				curr_anim->reset();
				set_state(MOVING_ALIEN);
			}
			
			process();
			
			if (shot_at || touched_player)
			{
				kill();
				break;
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



