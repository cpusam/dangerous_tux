#include "gyroalien.hpp"

#ifndef USE_SDL2
	CGyroAlien::CGyroAlien ( CPlayer * p, SVect i_p, int t )
#else
	CGyroAlien::CGyroAlien ( SDL_Renderer * r, CPlayer * p, SVect i_p, int t )
#endif
{
	id = "gyroalien";
	player = p;
	map = player->get_map();
	dim = (SDL_Rect){10,9,15,15};
	
	// define os tiles de colisão completa
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
	// à direita
	c_point.push_back(SVect(dim.x + dim.w, dim.y));
	c_point.push_back(SVect(dim.x + dim.w, dim.y + dim.h));
	// à esquerda
	c_point.push_back(SVect(dim.x, dim.y));
	c_point.push_back(SVect(dim.x, dim.y + dim.h));
	// abaixo
	c_point.push_back(SVect(dim.x, dim.y + dim.h));
	c_point.push_back(SVect(dim.x + dim.w, dim.y + dim.h));
	// acima
	c_point.push_back(SVect(dim.x, dim.y));
	c_point.push_back(SVect(dim.x + dim.w, dim.y));
	// centro de colisão
	c_point.push_back(SVect(dim.x + dim.w/2, dim.y + dim.h/2));
	
	anim.resize(8);
	curr_anim = &anim[0];
	
	SDL_Surface * left = 0, * right = 0;
	#if _WIN32 || _WIN64
		char path_left[FILENAME_MAX];
		char path_right[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path_left, "%s\\images\\circlealien_left.png", p2);
			sprintf(path_right, "%s\\images\\circlealien_right.png", p2);
		#else
			sprintf(path_left, "%s\\dangeroustux\\images\\circlealien_left.png", PREFIX);
			sprintf(path_right, "%s\\dangeroustux\\images\\circlealien_right.png", PREFIX);
		#endif
	#else
		char path_left[1024];
		char path_right[1024];
		#ifndef PREFIX
			sprintf(path_left, "./images/circlealien_left.png");
			sprintf(path_right, "./images/circlealien_right.png");
		#else
			sprintf(path_left, "%s/share/games/dangeroustux/images/circlealien_left.png", PREFIX);
			sprintf(path_right, "%s/share/games/dangeroustux/images/circlealien_right.png", PREFIX);
		#endif
	#endif
	
	#ifndef USE_SDL2
		left = optimize_surface_alpha(IMG_Load(path_left));
		if (!left)
			throw "CGyroAlien: não foi possível carregar circlealien_left.png\n";
		
		// Sentido anti-horário
		// animação de indo para esquerda pregado em cima do tile
		anim[0].add_frame(left, (SDL_Rect){0,0,48,48}, 1);
		// animação de indo para esquerda pregado em baixo do tile
		anim[1].add_frame(left, (SDL_Rect){0,48*2,48,48}, 1);
	
		// animação de indo para baixo pregado a esquerda do tile
		anim[2].add_frame(left, (SDL_Rect){0,48,48,48}, 1);
		// animação de indo para baixo pregado a direita do tile
		anim[3].add_frame(right, (SDL_Rect){0,0,48,48}, 1);
	
	  right = optimize_surface_alpha(IMG_Load(path_right));
		if (!right)
			throw "CGyroAlien: não foi possível carregar circlealien_right.png\n";
	
		// Sentido horário
		// animação de indo para direita pregado em cima do tile
		anim[4].add_frame(right, (SDL_Rect){0,0,48,48}, 1);
		// animação de indo para direita pregado em baixo do tile
		anim[5].add_frame(right, (SDL_Rect){0,48*2,48,48}, 1);
	
		// animação de indo para cima pregado a esquerda do tile
		anim[6].add_frame(right, (SDL_Rect){0,48*3,48,48}, 1);
		// animação de indo para cima pregado a direita do tile
		anim[7].add_frame(left, (SDL_Rect){0,48*3,48,48}, 1);
	#else
		SDL_Texture * texture = IMG_LoadTexture(r, path_left);
		if (!texture)
			throw "CGyroAlien: não foi possível carregar circlealien.png\n";

		// animação de indo para esquerda pregado em cima do tile
		anim[0].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
		// animação de indo para esquerda pregado em baixo do tile
		anim[1].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
	
		// animação de indo para baixo pregado a esquerda do tile
		anim[2].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
		// animação de indo para baixo pregado a direita do tile
		anim[3].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
	
		// animação de indo para direita pregado em cima do tile
		anim[4].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
		// animação de indo para direita pregado em baixo do tile
		anim[5].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
	
		// animação de indo para cima pregado a esquerda do tile
		anim[6].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
		// animação de indo para cim pregado a direita do tile
		anim[7].add_frame(texture, (SDL_Rect){0,0,36,35}, 1);
	#endif
	
	tile = t;
	switch (tile)
	{
		case 'A': // pega a esquerda do alien e direita do tile
			i_p.y += (map->get_tilesize() - dim.h)/2;
			vel.x = -map->get_tilesize()/2;
			pos = i_p;
			collision_hor();
			i_p.x = pos.x;
			dir_x = LEFT_ALIEN;
			
			if (player->get_pos().y < i_p.y)
				dir_y = UP_ALIEN;
			else
				dir_y = DOWN_ALIEN;
			
			dir = dir_y;
			break;
		
		case 'B': // pega abaixo do alien e acima do tile
			i_p.x += (map->get_tilesize() - dim.w)/2;
			i_p.y += map->get_tilesize() - dim.h;
			
			vel.y = map->get_tilesize()/2;
			pos = i_p;
			collision_ver();
			i_p = pos;
			
			if (player->get_pos().x < i_p.x)
				dir_x = LEFT_ALIEN;
			else
				dir_x = RIGHT_ALIEN;

			dir_y = DOWN_ALIEN;
			dir = dir_x;
			break;
		
		case 'C': // pega a direita do alien e a esquerda do tile
			i_p.x += map->get_tilesize() - dim.w;
			i_p.y += (map->get_tilesize() - dim.h)/2;
			dir_x = RIGHT_ALIEN;
			
			pos = i_p;
			vel.x = map->get_tilesize()/2;
			collision_hor();
			i_p = pos;

			if (player->get_pos().y < i_p.y)
				dir_y = UP_ALIEN;
			else
				dir_y = DOWN_ALIEN;

			dir = dir_y;
			break;
		
		case 'D': // pega acima do alien e abaixo do tile
			i_p.x += (map->get_tilesize() - dim.w)/2;
			
			pos = i_p;
			vel.y = -map->get_tilesize()/2;
			collision_ver();
			i_p = pos;
			
			if (player->get_pos().x < i_p.x)
				dir_x = LEFT_ALIEN;
			else
				dir_x = RIGHT_ALIEN;

			dir_y = UP_ALIEN;
			dir = dir_x;
			break;
		
		default:
			throw "CGyroAlien: tile não identificado\n";
	}
	
	init_pos = i_p;
}

CGyroAlien::~CGyroAlien (  )
{
	#ifndef USE_SDL2
		anim[0].destroy_surfaces();
	#else
		anim[0].destroy_textures();
	#endif
}


void CGyroAlien::reset_pos (  )
{
	pos = init_pos;
	
	switch (tile)
	{
		case 'A': // pega a esquerda do alien e direita do tile
			dir_x = LEFT_ALIEN;
			
			if (player->get_pos().y < pos.y)
				dir_y = UP_ALIEN;
			else
				dir_y = DOWN_ALIEN;
			
			dir = dir_y;
			break;
		
		case 'B': // pega abaixo do alien e acima do tile
			if (player->get_pos().x < pos.x)
				dir_x = LEFT_ALIEN;
			else
				dir_x = RIGHT_ALIEN;

			dir_y = DOWN_ALIEN;
			dir = dir_x;
			break;
		
		case 'C': // pega a direita do alien e a esquerda do tile
			dir_x = RIGHT_ALIEN;

			if (player->get_pos().y < pos.y)
				dir_y = UP_ALIEN;
			else
				dir_y = DOWN_ALIEN;

			dir = dir_y;
			break;
		
		case 'D': // pega acima do alien e abaixo do tile
			if (player->get_pos().x < pos.x)
				dir_x = LEFT_ALIEN;
			else
				dir_x = RIGHT_ALIEN;

			dir_y = UP_ALIEN;
			dir = dir_x;
			break;
		
		default:
			throw "CGyroAlien: tile não identificado\n";
	}
}
	
void CGyroAlien::reset (  )
{
	reset_pos();
	set_state(STOPED_ALIEN);
}

void CGyroAlien::input ( SDL_Event & event )
{
	
}

void CGyroAlien::process (  )
{
}

int CGyroAlien::collision_hor (  )
{
	int ret = 0;
	float p;
	SVect cp;
	SDL_Rect d = map->get_dimension();

	if (vel.x == 0)
		return ret;
	
	for (int i = 0; i < 4; i++)
	{
		cp.x = pos.x + c_point[i].x + vel.x;
		cp.y = pos.y + c_point[i].y;
		
		if (!pointbox(cp, d))
			continue;

		if (has_coll_tile(map->get_tile(cp.x, cp.y)))
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
	}

	return ret;
}

int CGyroAlien::collision_ver (  )
{
	int ret = 0;
	float p;
	SVect cp;
	SDL_Rect d = map->get_dimension();
	
	if (vel.y == 0)
		return ret;

	for (int i = 4; i < 8; i++)
	{
		cp.x = pos.x + c_point[i].x;
		cp.y = pos.y + c_point[i].y + vel.y;
		
		if (!pointbox(cp, d))
			continue;
		
		if (has_coll_tile(map->get_tile(cp.x, cp.y)))
		{
			if (i < 6)
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
	}
	
	return ret;
}

bool CGyroAlien::has_coll_tile ( int t )
{
	for (int i(0); i < coll_tiles.size(); i++)
		if (t == coll_tiles[i])
			return true;
	
	return false;
}

bool CGyroAlien::ground_right (  )
{
	SVect p;
	
	for (int i(0); i < 2; i++)
	{
		p = pos + c_point[i];
		p.x += 1;
		
		if (!pointbox(p, map->get_dimension()))
			return false;
	
		if (has_coll_tile(map->get_tile(p.x, p.y)))
			return true;
	}
	
	return false;
}

bool CGyroAlien::ground_left (  )
{
	SVect p;
	
	for (int i(2); i < 4; i++)
	{
		p = pos + c_point[i];
		p.x -= 1;
		
		if (!pointbox(p, map->get_dimension()))
			return false;
	
		if (has_coll_tile(map->get_tile(p.x, p.y)))
			return true;
	}
	
	return false;
}

bool CGyroAlien::ground_down (  )
{
	SVect p;
	
	for (int i(4); i < 6; i++)
	{
		p = pos + c_point[i];
		p.y += 1;
		
		if (!pointbox(p, map->get_dimension()))
			return false;
	
		if (has_coll_tile(map->get_tile(p.x, p.y)))
			return true;
	}
	
	return false;
}

bool CGyroAlien::ground_up (  )
{
	SVect p;
	
	for (int i(6); i < 8; i++)
	{
		p = pos + c_point[i];
		p.y -= 1;
		
		if (!pointbox(p, map->get_dimension()))
			return false;
	
		if (has_coll_tile(map->get_tile(p.x, p.y)))
			return true;
	}
	
	return false;
}

#ifndef USE_SDL2
	void CGyroAlien::draw ( CCamera * cam, SDL_Surface * screen )
#else
	void CGyroAlien::draw ( CCamera * cam, SDL_Renderer * renderer )
#endif
{
	#ifndef USE_SDL2
		if (curr_anim)
			curr_anim->draw(pos.x, pos.y, cam, screen);
	#else
		if (curr_anim)
			curr_anim->draw(pos.x, pos.y, cam, renderer);
	#endif
	
	if (get_state() == INACTIVE_ALIEN)
		return;
	
	if (get_state() == STOPED_ALIEN)
	{
		switch (dir)
		{
			case LEFT_ALIEN:
				if (dir_y == DOWN_ALIEN)
					curr_anim = &anim[0];
				else if (dir_y == UP_ALIEN)
					curr_anim = &anim[1];
				break;
				
			case RIGHT_ALIEN:
				if (dir_y == DOWN_ALIEN)
					curr_anim = &anim[4];
				else if (dir_y == UP_ALIEN)
					curr_anim = &anim[5];
				break;
				
			case DOWN_ALIEN:
				if (dir_x == RIGHT_ALIEN)
					curr_anim = &anim[2];
				else if (dir_x == LEFT_ALIEN)
					curr_anim = &anim[3];
				break;
				
			case UP_ALIEN:
				if (dir_x == RIGHT_ALIEN)
					curr_anim = &anim[6];
				else if (dir_x == LEFT_ALIEN)
					curr_anim = &anim[7];
				break;
			
			default:
				throw "CGyroAlien: direção desconhecida\n";
		}
		
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
	
		c.x = init_pos.x + c_point[8].x;
		c.y = init_pos.y + c_point[8].y;
		c.w = dim.w;
		c.h = dim.h;

		if (!boudingbox(a, b) && !boudingbox(c, a))
		{
			reset();
		}
	}
}

int CGyroAlien::update (  )
{
	int coll_ver, coll_hor;
	SVect p;
	
	switch (get_state())
	{
		case STOPED_ALIEN:
			
			break;
	
		case MOVING_ALIEN:
			switch (dir)
			{
				case LEFT_ALIEN:
					acc.x = -config.circle_acc.x;
					vel.x += acc.x;
					if (vel.x > config.circle_vel_max.x)
						vel.x = config.circle_vel_max.x;
					else if (vel.x < -config.circle_vel_max.x)
						vel.x = -config.circle_vel_max.x;
					
					pos.x += vel.x;
					p.x = pos.x + dim.x;
					p.y = pos.y + dim.y + dim.h/2;
					
					if (pointbox(p, map->get_dimension()) && has_coll_tile(map->get_tile(p.x, p.y)))
					{
						acc.zero();
						vel.zero();
						int x = ((int(p.x) / map->get_tilesize()) + 1) * map->get_tilesize();
						int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();
						
						pos.x = x - dim.x;
						if (dir_y == DOWN_ALIEN)
						{
							pos.y = y + (map->get_tilesize() - (dim.y + dim.h));
							dir_y = UP_ALIEN;
						}
						else if (dir_y == UP_ALIEN)
						{
							pos.y = y - dim.y;
							dir_y = DOWN_ALIEN;
						}
						
						dir_x = LEFT_ALIEN;
						dir = dir_y;
						break;
					}
					
					if (dir_y == DOWN_ALIEN)
					{
						// se não for chão abaixo do alien
						if (!ground_down())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_x = RIGHT_ALIEN;
							dir = dir_y;
							pos.x = x + (map->get_tilesize() - (dim.x + dim.w));
							pos.y = y + (map->get_tilesize() - (dim.y + dim.h));
						}
					}
					else if (dir_y == UP_ALIEN)
					{
						// se não for chão acima do alien
						if (!ground_up())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_x = RIGHT_ALIEN;
							dir = dir_y;
							pos.x = x + (map->get_tilesize() - (dim.x + dim.w));
							pos.y = y - dim.y;
						}
					}
					
					/*
					if (dir_y == DOWN_ALIEN)
						curr_anim = &anim[0];
					else if (dir_y == UP_ALIEN)
						curr_anim = &anim[1];
					
					curr_anim->update();
					*/
					break;
				
				case RIGHT_ALIEN:
					acc.x = config.circle_acc.x;
					
					vel.x += acc.x;
					if (vel.x > config.circle_vel_max.x)
						vel.x = config.circle_vel_max.x;
					else if (vel.x < -config.circle_vel_max.x)
						vel.x = -config.circle_vel_max.x;
					
					pos.x += vel.x;
					p.x = pos.x + dim.x + dim.w;
					p.y = pos.y + dim.y + dim.h/2;
					if (pointbox(p, map->get_dimension()) && has_coll_tile(map->get_tile(p.x, p.y)))
					{
						acc.zero();
						vel.zero();
						int x = ((int(p.x) / map->get_tilesize()) - 1) * map->get_tilesize();
						int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();
						
						pos.x = x + (map->get_tilesize() - (dim.x + dim.w));
						if (dir_y == DOWN_ALIEN)
						{
							pos.y = y + (map->get_tilesize() - (dim.y + dim.h));
							dir_y = UP_ALIEN;
						}
						else if (dir_y == UP_ALIEN)
						{
							pos.y = y - dim.y;
							dir_y = DOWN_ALIEN;
						}
						
						dir_x = RIGHT_ALIEN;
						dir = dir_y;
						break;
					}
					
					if (dir_y == DOWN_ALIEN)
					{
						// se não for chão abaixo do alien
						if (!ground_down())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_x = LEFT_ALIEN;
							dir = dir_y;
							pos.x = x - dim.x;
							pos.y = y + (map->get_tilesize() - (dim.y + dim.h));
						}
					}
					else if (dir_y == UP_ALIEN)
					{
						// se não for chão acima do alien
						if (!ground_up())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_x = LEFT_ALIEN;
							dir = dir_y;
							pos.x = x - dim.x;
							pos.y = y - dim.y;
						}
					}
					break;
				
				case DOWN_ALIEN:
					acc.y = config.circle_acc.y;
					
					vel.y += acc.y;
					if (vel.y > config.circle_vel_max.y)
						vel.y = config.circle_vel_max.y;
					else if (vel.y < -config.circle_vel_max.y)
						vel.y = -config.circle_vel_max.y;
					
					pos.y += vel.y;
					p.x = pos.x + dim.x + dim.w/2;
					p.y = pos.y + dim.y + dim.h;
					
					if (pointbox(p, map->get_dimension()) && has_coll_tile(map->get_tile(p.x, p.y)))
					{
						acc.zero();
						vel.zero();
						int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
						int y = (int(p.y) / map->get_tilesize() - 1) * map->get_tilesize();
						
						if (dir_x == LEFT_ALIEN)
						{
							pos.x = x - dim.x;
							dir_x = RIGHT_ALIEN;
						}
						else if (dir_x == RIGHT_ALIEN)
						{
							pos.x = x + (map->get_tilesize() - (dim.x + dim.w));
							dir_x = LEFT_ALIEN;
						}
						pos.y = y + (map->get_tilesize() - (dim.y + dim.h));

						dir_y = DOWN_ALIEN;
						dir = dir_x;
						break;
					}
					
					if (dir_x == LEFT_ALIEN)
					{
						// se não for chão a esquerda do alien
						if (!ground_left())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_y = UP_ALIEN;
							dir = dir_x;
							pos.x = x - dim.x;
							pos.y = y - dim.y;
						}
					}
					else if (dir_x == RIGHT_ALIEN)
					{
						// se não for chão acima do alien
						if (!ground_right())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_y = UP_ALIEN;
							dir = dir_x;
							pos.x = x + (map->get_tilesize() - (dim.x + dim.w));
							pos.y = y - dim.y;
						}
					}
					break;
				
				case UP_ALIEN:
					acc.y = -config.circle_acc.y;
					
					vel.y += acc.y;
					if (vel.y > config.circle_vel_max.y)
						vel.y = config.circle_vel_max.y;
					else if (vel.y < -config.circle_vel_max.y)
						vel.y = -config.circle_vel_max.y;
					
					pos.y += vel.y;
					p.x = pos.x + dim.x + dim.w/2;
					p.y = pos.y + dim.y;
					
					if (pointbox(p, map->get_dimension()) && has_coll_tile(map->get_tile(p.x, p.y)))
					{
						acc.zero();
						vel.zero();
						int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
						int y = (int(p.y) / map->get_tilesize() + 1) * map->get_tilesize();
						
						if (dir_x == LEFT_ALIEN)
						{
							pos.x = x - dim.x;
							dir_x = RIGHT_ALIEN;
						}
						else if (dir_x == RIGHT_ALIEN)
						{
							pos.x = x + (map->get_tilesize()  - (dim.x + dim.w));
							dir_x = LEFT_ALIEN;
						}
						pos.y = y - dim.y;
						
						dir_y = UP_ALIEN;
						dir = dir_x;
						break;
					}
					
					if (dir_x == LEFT_ALIEN)
					{
						// se não for chão a esquerda do alien
						if (!ground_left())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_y = DOWN_ALIEN;
							dir = dir_x;
							pos.x = x - dim.x;
							pos.y = y + (map->get_tilesize() - (dim.y + dim.h));
						}
					}
					else if (dir_x == RIGHT_ALIEN)
					{
						// se não for chão acima do alien
						if (!ground_right())
						{
							acc.zero();
							vel.zero();
							p = pos + c_point[8];
							int x = (int(p.x) / map->get_tilesize()) * map->get_tilesize();
							int y = (int(p.y) / map->get_tilesize()) * map->get_tilesize();

							dir_y = DOWN_ALIEN;
							dir = dir_x;
							pos.x = x + (map->get_tilesize() - (dim.x + dim.w));
							pos.y = y + (map->get_tilesize() - (dim.y + dim.h));
						}
					}
					break;
			}
			break;
		
		case DYING_ALIEN:
			
			break;
		
		case INACTIVE_ALIEN:
			break;
	}
	
	return get_state();
}


