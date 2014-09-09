#include "player.hpp"

#ifndef USE_SDL2
	CPlayer::CPlayer (  ): jetpack(100.0f, 0.1f, false)
#else
	CPlayer::CPlayer ( SDL_Renderer * r ): jetpack(100.0f, 0.1f, false)
#endif
{
	#ifndef USE_SDL2
		SDL_Surface * sright, * sleft;
	#else
		SDL_Texture * tright = 0, * tleft = 0;
	#endif
	dir = RIGHT_PLAYER;
	up_key = down_key = left_key = right_key = 0;
	jump_key = gun_key = jetpack_key = 0;
	map = 0;
	lives = 2;
	final_pos = 0;
	set_state(WAITING); // vai para esperando ser controlado
	kernel = false;
	shot_at = touched_alien = false;
	limit = (SDL_Rect){0,0,0,0};
	SVect pos_d[2] = {SVect(5,19),SVect(42,19)};
	gun.set_pos_dir(pos_d);
	gun.shot.set_id("player_shot");
	set_id("player");
	
	if (SDL_NumJoysticks() == 0)
		has_joystick = false;
	else
	{
		SDL_JoystickEventState(SDL_ENABLE);
		joystick = SDL_JoystickOpen(0);
		has_joystick = true;
	}

	// define os itens para coletar
	item.push_back('d');
	item.push_back('e');
	item.push_back('f');
	item.push_back('g');
	item.push_back('q');
	item.push_back('r');
	item.push_back('t');
	item.push_back('K');
	
	// define os tiles para colisão completa
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
	c_point.push_back(SVect(31,6));
	c_point.push_back(SVect(31,43));
	// colisão a esquerda
	c_point.push_back(SVect(15,6));
	c_point.push_back(SVect(15,43));
	// colisão abaixo
	c_point.push_back(SVect(15,43));
	c_point.push_back(SVect(31,43));
	// colisão acima
	c_point.push_back(SVect(15,6));
	c_point.push_back(SVect(31,6));
	// centro da colisão
	c_point.push_back(SVect(24,22));
	
	// seta rect de colisão
	dim.x = 15;
	dim.y = 6;
	dim.w = 16;
	dim.h = 37;
	
	anim.resize(16);

	
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path, "%s\\images\\tux_right.png", p2);
			#ifndef USE_SDL2
				sright = optimize_surface_alpha(IMG_Load(path));
			#else
				tright = IMG_LoadTexture(r, path);
			#endif
			
			sprintf(path, "%s\\images\\tux_left.png", p2);
			#ifndef USE_SDL2
				sleft = optimize_surface_alpha(IMG_Load(path));
			#else
				tleft = IMG_LoadTexture(r, path);
			#endif
		#else
			sprintf(path, "%s\\dangeroustux\\images\\tux_right.png", PREFIX);
			#ifndef USE_SDL2
				sright = optimize_surface_alpha(IMG_Load(path));
			#else
				tright = IMG_LoadTexture(r, path);
			#endif
			
			sprintf(path, "%s\\dangeroustux\\images\\tux_left.png", PREFIX);
			#ifndef USE_SDL2
				sleft = optimize_surface_alpha(IMG_Load(path));
			#else
				tleft = IMG_LoadTexture(r, path);
			#endif
		#endif	
	#else
		char path[1024];
		#ifndef PREFIX
			sprintf(path, "./images/tux_right.png");
			#ifndef USE_SDL2
				sright = optimize_surface_alpha(IMG_Load(path));
			#else
				tright = IMG_LoadTexture(r, path);
			#endif
			
			sprintf(path, "./images/tux_left.png");
			#ifndef USE_SDL2
				sleft = optimize_surface_alpha(IMG_Load(path));
			#else
				tleft = IMG_LoadTexture(r, path);
			#endif
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/tux_right.png", PREFIX);
			#ifndef USE_SDL2
				sright = optimize_surface_alpha(IMG_Load(path));
			#else
				tright = IMG_LoadTexture(r, path);
			#endif

			sprintf(path, "%s/share/games/dangeroustux/images/tux_left.png", PREFIX);
			#ifndef USE_SDL2
				sleft = optimize_surface_alpha(IMG_Load(path));
			#else
				tleft = IMG_LoadTexture(r, path);
			#endif
		#endif			
	#endif
	
	#ifndef USE_SDL2
		if (!sright)
				throw SDL_GetError();
	
		if (!sleft)
			throw SDL_GetError();
		// virado para direita
		// parado
		anim[0].add_frame(sright, (SDL_Rect){48 * 3,0,48,48}, 1);
		// caminhando
		anim[1].add_frame(sright, (SDL_Rect){0,0,48,48}, 3);
		anim[1].add_frame(sright, (SDL_Rect){48,0,48,48}, 5);
		anim[1].add_frame(sright, (SDL_Rect){48*2,0,48,48}, 3);
		// pulando ou caindo
		anim[2].add_frame(sright, (SDL_Rect){0,48*2,48,48}, 1);
		// parado nas árvores
		anim[3].add_frame(sright, (SDL_Rect){48*2,48,48,48}, 1);
		// caminhando nas árvores
		anim[4].add_frame(sright, (SDL_Rect){0,48,48,48}, 4);
		anim[4].add_frame(sright, (SDL_Rect){48,48,48,48}, 4);
		// no jetpack
		anim[5].add_frame(sright, (SDL_Rect){0,48*3,48,48}, 2);
		anim[5].add_frame(sright, (SDL_Rect){48,48*3,48,48}, 2);
		anim[5].add_frame(sright, (SDL_Rect){48*2,48*3,48,48}, 2);
		// atirando
		anim[14].add_frame(sright, (SDL_Rect){0,48*6,48,48}, 3);
		anim[14].add_frame(sright, (SDL_Rect){48,48*6,48,48}, 3);
		anim[14].add_frame(sright, (SDL_Rect){48*2,48*6,48,48}, 3);

		// virado para esquerda
		// parado
		anim[6].add_frame(sleft, (SDL_Rect){48 * 3,0,48,48}, 1);
		// caminhando
		anim[7].add_frame(sleft, (SDL_Rect){0,0,48,48}, 3);
		anim[7].add_frame(sleft, (SDL_Rect){48,0,48,48}, 5);
		anim[7].add_frame(sleft, (SDL_Rect){48*2,0,48,48}, 3);
		// pulando ou caindo
		anim[8].add_frame(sleft, (SDL_Rect){0,48*2,48,48}, 1);
		// parado nas árvores
		anim[9].add_frame(sleft, (SDL_Rect){48*2,48,48,48}, 1);
		// caminhando nas árvores
		anim[10].add_frame(sleft, (SDL_Rect){0,48,48,48}, 4);
		anim[10].add_frame(sleft, (SDL_Rect){48,48,48,48}, 4);
		// no jetpack
		anim[11].add_frame(sleft, (SDL_Rect){0,48*3,48,48}, 2);
		anim[11].add_frame(sleft, (SDL_Rect){48,48*3,48,48}, 2);
		anim[11].add_frame(sleft, (SDL_Rect){48*2,48*3,48,48}, 2);
		// atirando
		anim[15].add_frame(sleft, (SDL_Rect){0,48*6,48,48}, 3);
		anim[15].add_frame(sleft, (SDL_Rect){48,48*6,48,48}, 3);
		anim[15].add_frame(sleft, (SDL_Rect){48*2,48*6,48,48}, 3);
	
		// morrendo
		anim[12].add_frame(sright, (SDL_Rect){0,48*4,48,48}, 6);
		anim[12].add_frame(sright, (SDL_Rect){48,48*4,48,48}, 6);
		anim[12].add_frame(sright, (SDL_Rect){48*2,48*4,48,48}, 10);
		// esperando ser controlado
		anim[13].add_frame(sright, (SDL_Rect){0,48*5,48,48}, 8);
		anim[13].add_frame(sright, (SDL_Rect){0,0,0,0}, 4);
	#else
		if (!tright)
				throw SDL_GetError();
	
		if (!tleft)
			throw SDL_GetError();
		
		// virado para direita
		// parado
		anim[0].add_frame(tright, (SDL_Rect){48 * 3,0,48,48}, 1);
		// caminhando
		anim[1].add_frame(tright, (SDL_Rect){0,0,48,48}, 3);
		anim[1].add_frame(tright, (SDL_Rect){48,0,48,48}, 5);
		anim[1].add_frame(tright, (SDL_Rect){48*2,0,48,48}, 3);
		// pulando ou caindo
		anim[2].add_frame(tright, (SDL_Rect){0,48*2,48,48}, 1);
		// parado nas árvores
		anim[3].add_frame(tright, (SDL_Rect){48*2,48,48,48}, 1);
		// caminhando nas árvores
		anim[4].add_frame(tright, (SDL_Rect){0,48,48,48}, 4);
		anim[4].add_frame(tright, (SDL_Rect){48,48,48,48}, 4);
		// no jetpack
		anim[5].add_frame(tright, (SDL_Rect){0,48*3,48,48}, 2);
		anim[5].add_frame(tright, (SDL_Rect){48,48*3,48,48}, 2);
		anim[5].add_frame(tright, (SDL_Rect){48*2,48*3,48,48}, 2);
		// atirando
		anim[14].add_frame(tright, (SDL_Rect){0,48*6,48,48}, 3);
		anim[14].add_frame(tright, (SDL_Rect){48,48*6,48,48}, 3);
		anim[14].add_frame(tright, (SDL_Rect){48*2,48*6,48,48}, 3);

		// virado para esquerda
		// parado
		anim[6].add_frame(tleft, (SDL_Rect){48 * 3,0,48,48}, 1);
		// caminhando
		anim[7].add_frame(tleft, (SDL_Rect){0,0,48,48}, 3);
		anim[7].add_frame(tleft, (SDL_Rect){48,0,48,48}, 5);
		anim[7].add_frame(tleft, (SDL_Rect){48*2,0,48,48}, 3);
		// pulando ou caindo
		anim[8].add_frame(tleft, (SDL_Rect){0,48*2,48,48}, 1);
		// parado nas árvores
		anim[9].add_frame(tleft, (SDL_Rect){48*2,48,48,48}, 1);
		// caminhando nas árvores
		anim[10].add_frame(tleft, (SDL_Rect){0,48,48,48}, 4);
		anim[10].add_frame(tleft, (SDL_Rect){48,48,48,48}, 4);
		// no jetpack
		anim[11].add_frame(tleft, (SDL_Rect){0,48*3,48,48}, 2);
		anim[11].add_frame(tleft, (SDL_Rect){48,48*3,48,48}, 2);
		anim[11].add_frame(tleft, (SDL_Rect){48*2,48*3,48,48}, 2);
		// atirando
		anim[15].add_frame(tleft, (SDL_Rect){0,48*6,48,48}, 3);
		anim[15].add_frame(tleft, (SDL_Rect){48,48*6,48,48}, 3);
		anim[15].add_frame(tleft, (SDL_Rect){48*2,48*6,48,48}, 3);
	
		// morrendo
		anim[12].add_frame(tright, (SDL_Rect){0,48*4,48,48}, 6);
		anim[12].add_frame(tright, (SDL_Rect){48,48*4,48,48}, 6);
		anim[12].add_frame(tright, (SDL_Rect){48*2,48*4,48,48}, 10);
		// esperando ser controlado
		anim[13].add_frame(tright, (SDL_Rect){0,48*5,48,48}, 8);
		anim[13].add_frame(tright, (SDL_Rect){0,0,0,0}, 4);
	#endif
	
	curr_anim = &anim[13];
}

CPlayer::	~CPlayer ( )
{
	#ifndef USE_SDL2
		anim[0].destroy_surfaces();
		anim[6].destroy_surfaces();
	#else
		anim[0].destroy_textures();
		anim[6].destroy_textures();
	#endif
	
	if (has_joystick)
		SDL_JoystickClose(joystick);
} 

SVect CPlayer::get_center (  )
{
	return c_point[8];
}

void CPlayer::set_kernel ( bool h )
{
	kernel = h;
}

CTileMap * CPlayer::get_map (  )
{
	return map;
}

void CPlayer::set_map ( CTileMap * m )
{
	map = m;
	gun.shot.set_map(m);
	limit = map->get_dimension();
}

void CPlayer::set_lives ( int l )
{
	lives = l;
}

int CPlayer::get_lives (  )
{
	return lives;
}

void CPlayer::set_respawn ( SVect p )
{
	respawn = p;
}

void CPlayer::kill (  )
{
	acc.zero();
	vel.zero();
	curr_anim = &anim[12]; // morrendo para esquerda
	curr_anim->reset();
	set_state(DYING_PLAYER);
}

void CPlayer::reset_gamemenu (  )
{
	vel.zero();
	acc.zero();
	dir = RIGHT_PLAYER;
	pos = respawn;
	kernel = true;
	shot_at = touched_alien = false;
	curr_anim = &anim[0]; // animação de parado para direita
	left_key = right_key = up_key = down_key = 0;
	jump_key = gun_key = jetpack_key = 0;
	set_state(STANDING);
}

void CPlayer::reset (  )
{
	vel.zero();
	acc.zero();
	pos = respawn;
	shot_at = touched_alien = false;
	curr_anim = &anim[13]; // animação de esperando controle
	left_key = right_key = up_key = down_key = 0;
	jump_key = gun_key = jetpack_key = 0;
	set_state(WAITING);
}

void CPlayer::set_transition ( CTileMap * m, SVect p, float f_pos )
{
	set_map(m);
	vel.zero();
	acc.zero();
	pos = p;
	collision_ver();
	final_pos = f_pos;
	dir = RIGHT_PLAYER;
	curr_anim = &anim[1]; // caminhando para direita
	curr_anim->reset();
	set_state(WALKING_TRANSITION);
}

bool CPlayer::has_coll_tile ( int tile )
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

bool CPlayer::ground (  )
{
	SVect cp;
	int tile;
	
	for (int i = 4; i < 6; i++)
	{
		cp.x = pos.x + c_point[i].x;
		cp.y = pos.y + c_point[i].y + 1;
		
		if (!pointbox(cp, map->get_dimension()))
			continue;
		
		tile = map->get_tile(cp.x, cp.y);
		if (has_coll_tile(tile))
			return true;
	}

	return false;
}

bool CPlayer::tree (  )
{
	int tile = map->get_tile(int(pos.x + c_point[8].x), int(pos.y + c_point[8].y));
	
	return (tile == 'i' || tile == 'j' || tile == 'k' || tile == 'l' || tile == 'm' || tile == 'n' || tile == 'o' || tile == 'p' || tile == 'q' || tile == 'r');
}

int CPlayer::collision_hor (  )
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
		
int CPlayer::collision_ver (  )
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

/*
	Verifica se tocou um tile que mata
*/
bool CPlayer::touch_bad_tile (  )
{
	for (int i(0); i < 8; i++)
		if ((i >= 4 && i < 6) || i == 1 || i == 3)
		{
			switch (map->get_tile(pos.x + c_point[i].x, pos.y + c_point[i].y - 18.0))
			{
				case 'o':
				case 'p':
				return true;
			}
		}
		else
		{
			switch (map->get_tile(pos.x + c_point[i].x, pos.y + c_point[i].y))
			{
				case 'o':
				case 'p':
				return true;
			}
		}

	return false;
}

/*
	Função que verifca e coleta itens
*/
bool CPlayer::collect_items (  )
{
	for (int i(0); i < item.size(); i++)
		for (int j(0); j < c_point.size(); j++)
			if (map->get_tile(pos.x + c_point[j].x, pos.y + c_point[j].y) == item[i])
			{
				// verifica qual item foi coletado e dá a pontuação
				switch (item[i])
				{
					case 'd':
					case 'e':
					case 'f':
					case 'g':
					case 't':
					case 'K': // pegou o item chave
						if (item[i] == 'K')
							kernel = true;
						score.collect_item(item[i]);
						break;

					case 'q': // pegou o jetpack
						jetpack.set_jetpack(true);
						break;

					case 'r': // pegou a arma
						gun.set_gun(true);
						break;
					default:
						break;
				}
				// deixa o tile 'vazio'
				map->set_tile(pos.x + c_point[j].x, pos.y + c_point[j].y, '.');
				return true;
			}
	
	return false;
}

bool CPlayer::has_kernel (  )
{
	return kernel;
}

/*
	função que verifica e faz entrar na porta se tiver a chave
*/
bool CPlayer::enter_door (  )
{
	if (kernel)
	{
		// se for a porta
		if (map->get_tile(pos.x + c_point[8].x, pos.y + c_point[8].y) == 'E')
		{
			kernel = false;
			return true;
		}
	}
	
	return false;
}

void CPlayer::process (  )
{
	shot_at = touched_alien = false;
	events = CGameEventManager::instance()->get_events(static_cast<CGameEntity *>(this));
	for (vector <SGameEvent>::iterator it = events.begin(); it != events.end(); it++)
	{
		if ((*it).event == "shot_at")
			shot_at = true;
		else if ((*it).event == "touched_alien")
			touched_alien = true;
	}
}

void CPlayer::input ( SDL_Event & event )
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_UP:
				up_key = 1;
				break;
			case SDLK_DOWN:
				down_key = 1;
				break;
			case SDLK_LEFT:
				left_key = 1;
				break;
			case SDLK_RIGHT:
				right_key = 1;
				break;
			case SDLK_s:
				gun_key = 1;
				break;
			case SDLK_a:
				jetpack_key = 1;
				break;
			default:
				break;
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_UP:
				up_key = 0;
				break;
			case SDLK_DOWN:
				down_key = 0;
				break;
			case SDLK_LEFT:
				left_key = 0;
				break;
			case SDLK_RIGHT:
				right_key = 0;
				break;
			case SDLK_s:
				gun_key = 0;
				break;
			case SDLK_a:
				jetpack_key = 0;
				break;
			default:
				break;
		}
	}
	
	if (!has_joystick)
		return;
	
	if (event.type == SDL_JOYAXISMOTION)
	{
		// se é no eixo X
		if (event.jaxis.axis == 0)
		{
			if (event.jaxis.value < 0)
			{
				left_key = 1;
			}
			else if (event.jaxis.value > 0)
			{
				right_key = 1;
			}
			else
				left_key = right_key = 0;
		}
		
		// se é no eixo Y
		if (event.jaxis.axis == 1)
		{
			if (event.jaxis.value < 0)
			{
				up_key = 1;
			}
			else if (event.jaxis.value > 0)
			{
				down_key = 1;
			}
			else
				up_key = down_key = 0;
		}
	}
	
	if (event.type == SDL_JOYBUTTONDOWN)
	{
		switch (event.jbutton.button)
		{
			case 1:
				jetpack_key = 1;
				break;
				
			case 2:
				jump_key = 1;
				break;

			case 3:
				gun_key = 1;
				break;
				
			default:
				break;
		}
	}
	else if (event.type == SDL_JOYBUTTONUP)
	{
		switch (event.jbutton.button)
		{
			case 1:
				jetpack_key = 0;
				break;
				
			case 2:
				jump_key = 0;
				break;

			case 3:
				gun_key = 0;
				break;
				
			default:
				break;
		}
	}
}

#ifndef USE_SDL2
void CPlayer::draw ( CCamera * cam, SDL_Surface * screen )
{
	if (curr_anim)
		curr_anim->draw(pos.x, pos.y, cam, screen);
}
#else
void CPlayer::draw ( CCamera * cam, SDL_Renderer * renderer )
{
	if (curr_anim)
		curr_anim->draw(pos.x, pos.y, cam, renderer);
}
#endif

int CPlayer::update (  )
{
	switch (get_state())
	{
		case WAITING: // esperando ser controlado
			if (left_key)
			{
				dir = LEFT_PLAYER;
				set_state(STANDING);
			}
			else if (right_key)
			{
				dir = RIGHT_PLAYER;
				set_state(STANDING);
			}
			else if (up_key)
			{
				dir = RIGHT_PLAYER;
				set_state(STANDING);
			}

			curr_anim->update();
			break;

		case STANDING: // parado
			if (ground())
			{
				if (left_key)
				{
					dir = LEFT_PLAYER;
					curr_anim = &anim[7]; // caminhando para esquerda
					curr_anim->reset();
					set_state(WALKING);
					break;
				}
				else if (right_key)
				{
					dir = RIGHT_PLAYER;
					curr_anim = &anim[1]; // caminhando para direita
					curr_anim->reset();
					set_state(WALKING);
					break;
				}
				
				if (up_key)
				{
					if (tree())
					{
						pos.x = (int(pos.x + c_point[8].x) / map->get_tilesize()) * map->get_tilesize();

						acc.zero();
						vel.zero();
						if (dir == RIGHT_PLAYER)
							curr_anim = &anim[3]; // parado na árvore para direita
						else
							curr_anim = &anim[9]; // parado na árvore para esquerda
						curr_anim->reset();
						set_state(STANDING_TREE);
						break;
					}
				}
				
				if (up_key || jump_key)
				{
					vel.y = -config.vel_max.y;
					collision_ver();
					pos.y += vel.y;
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[2]; // caindo ou pulando para direita
					else
						curr_anim = &anim[8]; // caindo ou pulando para esquerda
					curr_anim->reset();
					set_state(JUMPING);
					break;
				}
				
				if (jetpack_key)
					if (jetpack.has_jetpack() && jetpack.has_fuel())
					{
						jetpack_key = 0;
						vel.zero();
						acc.zero();
						if (dir == RIGHT_PLAYER)
							curr_anim = &anim[5]; // no jetpack para direita
						else
							curr_anim = &anim[11]; // no jetpack para esquerda
						set_state(JETPACK);
						break;
					}
				
				if (gun_key)
					if (gun.has_gun())
					{
						gun.fire(pos, dir, SVect(((dir == LEFT_PLAYER)? -config.shot_vel: config.shot_vel), 0));
						if (gun.was_used() && gun.shot.get_state() == 0)
						{
							if (dir == RIGHT_PLAYER)
								curr_anim = &anim[14]; // parado atirando para direita
							else
								curr_anim = &anim[15]; // parado atirando para esquerda
							
							curr_anim->reset();
							set_state(STANDING_FIRE);
							break;
						}
					}
			}
			else
			{
				vel.y = 0;
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[2]; // caindo ou pulando para direita
				else
					curr_anim = &anim[8]; // caindo ou pulando para esquerda
				curr_anim->reset();
				set_state(JUMPING);
				break;
			}
			
			collect_items();
			if (enter_door())
			{
				curr_anim = 0;
				kernel = false;
				gun.set_gun(false);
				jetpack.set_jetpack(false);
				set_state(FINISHED_LEVEL);
				break;
			}
			
			process();
			
			if (shot_at || touched_alien)
			{
				kill();
				break;
			}
			
			curr_anim->update();
			break;

		case WALKING: // caminhando
			if (ground())
			{
				if (left_key)
				{
					dir = LEFT_PLAYER;
					if (vel.x > 0)
						vel.x = 0;
					acc.x = -config.acc.x;
					curr_anim = &anim[7]; // caminhando para esquerda
				}
				else if (right_key)
				{
					dir = RIGHT_PLAYER;
					if (vel.x < 0)
						vel.x = 0;
					acc.x = config.acc.x;
					curr_anim = &anim[1]; // caminhando para direita
				}
				else
				{
					acc.zero();
					vel.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[0]; // parado para direita
					else
						curr_anim = &anim[6]; // parado para esquerda
					curr_anim->reset();
					set_state(STANDING);
					break;
				}
				
				if (up_key)
				{
					if (tree())
					{
						pos.x = (int(pos.x + c_point[8].x) / map->get_tilesize()) * map->get_tilesize();

						acc.zero();
						vel.zero();
						if (dir == RIGHT_PLAYER)
							curr_anim = &anim[3]; // parado na árvore para direita
						else
							curr_anim = &anim[9]; // parado na árvore para esquerda
						curr_anim->reset();
						set_state(STANDING_TREE);
						break;
					}
				}
				
				if (up_key || jump_key)
				{
					vel.y = -config.vel_max.y;
					collision_ver();
					pos.y += vel.y;
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[2]; // caindo ou pulando para direita
					else
						curr_anim = &anim[8]; // caindo ou pulando para esquerda
					curr_anim->reset();
					set_state(JUMPING);
					break;
				}
			}
			else
			{
				vel.y = 0;
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[2]; // caindo ou pulando para direita
				else
					curr_anim = &anim[8]; // caindo ou pulando para esquerda
				curr_anim->reset();
				set_state(JUMPING);
				break;
			}
			
			if (jetpack_key)
				if (jetpack.has_jetpack() && jetpack.has_fuel())
				{
					jetpack_key = 0;
					vel.zero();
					acc.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[5]; // no jetpack para direita
					else
						curr_anim = &anim[11]; // no jetpack para esquerda
					set_state(JETPACK);
					break;
				}
			
			if (gun_key)
				if (gun.has_gun())
				{
					gun.fire(pos, dir, SVect(((dir == LEFT_PLAYER)? -config.shot_vel: config.shot_vel), 0));
					if (gun.was_used() && gun.shot.get_state() == 0)
					{
						if (dir == RIGHT_PLAYER)
							curr_anim = &anim[14]; // parado atirando para direita
						else
							curr_anim = &anim[15]; // parado atirando para esquerda
						
						curr_anim->reset();
						acc.zero();
						vel.zero();
						set_state(STANDING_FIRE);
						break;
					}
				}
			
			vel.x += acc.x;
			if (vel.x > config.vel_max.x)
				vel.x = config.vel_max.x;
			else if (vel.x < -config.vel_max.x)
				vel.x = -config.vel_max.x;
			
			collision_ver();
			pos.y += vel.y;
			collision_hor();
			pos.x += vel.x;
			
			// limita o movimento no eixo X
			if (pos.x + c_point[0].x > limit.x + limit.w - 2)
				pos.x = limit.x + limit.w - c_point[0].x - 2;
			else if (pos.x + c_point[2].x < limit.x)
				pos.x = limit.x - c_point[2].x;
			
			process();
			
			if (shot_at || touched_alien)
			{
				kill();
				break;
			}
			
			collect_items();
			if (enter_door())
			{
				curr_anim = 0;
				kernel = false;
				gun.set_gun(false);
				jetpack.set_jetpack(false);
				set_state(FINISHED_LEVEL);
				break;
			}
			
			curr_anim->update();
			break;

		case JUMPING: // pulando ou caindo
			if (ground())
			{
				acc.zero();
				vel.zero();
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[0]; // parado para direita
				else
					curr_anim = &anim[6]; // parado para esquerda
				curr_anim->reset();
				set_state(STANDING);
				break;
			}
			
			if (up_key)
			{
				if (tree())
				{
					pos.x = (int(pos.x + c_point[8].x) / map->get_tilesize()) * map->get_tilesize();

					acc.zero();
					vel.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[3]; // parado na árvore para direita
					else
						curr_anim = &anim[9]; // parado na árvore para esquerda
					curr_anim->reset();
					set_state(STANDING_TREE);
					break;
				}
			}
			
			if (left_key)
			{
				dir = LEFT_PLAYER;
				acc.x = -config.acc.x;
				curr_anim = &anim[8]; // caindo ou pulando para esquerda
			}
			else if (right_key)
			{
				dir = RIGHT_PLAYER;
				acc.x = config.acc.x;
				curr_anim = &anim[2]; // caindo ou pulando para direita
			}
			else
			{
				acc.x = 0;
			}
			
			if (jetpack_key)
				if (jetpack.has_jetpack() && jetpack.has_fuel())
				{
					jetpack_key = 0;
					vel.zero();
					acc.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[5]; // no jetpack para direita
					else
						curr_anim = &anim[11]; // no jetpack para esquerda
					set_state(JETPACK);
					break;
				}
			
			if (gun_key)
				if (gun.has_gun())
				{
					gun.fire(pos, dir, SVect(((dir == LEFT_PLAYER)? -config.shot_vel: config.shot_vel), 0));
					if (gun.was_used() && gun.shot.get_state() == 0)
					{
						if (dir == RIGHT_PLAYER)
							curr_anim = &anim[14]; // parado atirando para direita
						else
							curr_anim = &anim[15]; // parado atirando para esquerda
						
						curr_anim->reset();
						set_state(JUMPING_FIRE);
						break;
					}
				}
			
			vel.x += acc.x;
			if (vel.x > config.vel_max.x)
				vel.x = config.vel_max.x;
			else if (vel.x < -config.vel_max.x)
				vel.x = -config.vel_max.x;

			vel.y += config.gravity;
			if (vel.y > config.vel_max.y)
				vel.y = config.vel_max.y;
			else if (vel.y < -config.vel_max.y)
				vel.y = -config.vel_max.y;
			
			collision_ver();
			pos.y += vel.y;
			collision_hor();
			pos.x += vel.x;
			
			// limita o movimento no eixo X
			if (pos.x + c_point[0].x > limit.x + limit.w - 2)
				pos.x = limit.x + limit.w - c_point[0].x - 2;
			else if (pos.x + c_point[2].x < limit.x)
				pos.x = limit.x - c_point[2].x;
			
			// limita o movimento no eixo Y
			if (pos.y > limit.y + limit.h) // se passou por baixo da tela
				pos.y = limit.y - map->get_tilesize(); // volte lá pra cima da fase

			process();
			
			if (touch_bad_tile() || shot_at || touched_alien)
			{
				kill();
				break;
			}
			
			collect_items();
			if (enter_door())
			{
				curr_anim = 0;
				kernel = false;
				gun.set_gun(false);
				jetpack.set_jetpack(false);
				set_state(FINISHED_LEVEL);
				break;
			}

			curr_anim->update();
			break;

		case JETPACK: // usando jet pack
			if (jetpack_key) // se é para desligar o jetpack
			{
				jetpack_key = 0;
				vel.zero();
				acc.zero();
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[0]; // parado para direita
				else
					curr_anim = &anim[6]; // parado para esquerda
				set_state(STANDING);
				break;
			}
			
			if (left_key)
			{
				dir = LEFT_PLAYER;
				if (vel.x > 0)
					vel.x = 0;
				acc.x = -config.acc_jetpack.x;
				curr_anim = &anim[11]; // no jet pack para esquerda
			}
			else if (right_key)
			{
				dir = RIGHT_PLAYER;
				if (vel.x < 0)
					vel.x = 0;
				acc.x = config.acc_jetpack.x;
				curr_anim = &anim[5]; // no jet pack para direita
			}
			else
			{
				acc.x = 0;
				vel.x = 0;
			}
			
			if (up_key)
			{
				if (vel.y > 0)
					vel.y = 0;
				acc.y = -config.acc_jetpack.y;
			}
			else if (down_key)
			{
				if (vel.y < 0)
					vel.y = 0;
				acc.y = config.acc_jetpack.y;
			}
			else
			{
				acc.y = 0;
				vel.y = 0;
			}

			// verifica se está sem combustÃ­vel
			if (jetpack.has_fuel() == false)
			{
				vel.zero();
				acc.zero();
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[0]; // parado para direita
				else
					curr_anim = &anim[6]; // parado para esquerda
				// retira o jetpack
				jetpack.set_jetpack(false);
				set_state(STANDING);
				break;
			}
			
			if (gun_key)
				if (gun.has_gun())
				{
					gun.fire(pos, dir, SVect(((dir == LEFT_PLAYER)? -config.shot_vel: config.shot_vel), 0));
				}
			
			vel.x += acc.x;
			if (vel.x > config.vel_max_jetpack)
				vel.x = config.vel_max_jetpack;
			else if (vel.x < -config.vel_max_jetpack)
				vel.x = -config.vel_max_jetpack;
			
			vel.y += acc.y;
			if (vel.y > config.vel_max_jetpack)
				vel.y = config.vel_max_jetpack;
			else if (vel.y < -config.vel_max_jetpack)
				vel.y = -config.vel_max_jetpack;
			
			collision_ver();
			pos.y += vel.y;
			collision_hor();
			pos.x += vel.x;

			// limita o movimento no eixo X
			if (pos.x + c_point[0].x > limit.x + limit.w - 2)
				pos.x = limit.x + limit.w - c_point[0].x - 2;
			else if (pos.x + c_point[2].x < limit.x)
				pos.x = limit.x - c_point[2].x;
			
			// limita o movimento no eixo Y
			if (pos.y > limit.y + limit.h) // se passou por baixo da tela
				pos.y = limit.y - map->get_tilesize(); // volte lá pra cima da fase
			else if (pos.y < limit.y - map->get_tilesize()) // se passar por cima
				pos.y = limit.y - map->get_tilesize(); // mantenha a posição
			
			process();
			
			if (touch_bad_tile() || shot_at || touched_alien)
			{
				kill();
				break;
			}
			
			collect_items();
			if (enter_door())
			{
				curr_anim = 0;
				kernel = false;
				gun.set_gun(false);
				jetpack.set_jetpack(false);
				set_state(FINISHED_LEVEL);
				break;
			}
			jetpack.update();
			
			curr_anim->update();
			break;
		
		case STANDING_TREE: // parado na árvore
			if (left_key || right_key || up_key || down_key)
			{
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[4]; // andando na árvore para direita
				else
					curr_anim = &anim[10]; // andando na árvore para esquerda
				curr_anim->reset();
				set_state(WALKING_TREE);
				break;
			}

			process();
			
			if (shot_at || touched_alien)
			{
				kill();
				break;
			}

			curr_anim->update();
			break;

		case WALKING_TREE: // andando na árvore
			if (!tree())
			{
				if (ground())
				{
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[0]; // parado para direita
					else
						curr_anim = &anim[6]; // parado para esquerda
				
					curr_anim->reset();
					set_state(STANDING);
					break;
				}
				
				if (left_key || right_key || up_key)
				{
					vel.y = -config.vel_max.y;
					collision_ver();
					pos.y += vel.y;
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[2]; // caindo ou pulando para direita
					else
						curr_anim = &anim[8]; // caindo ou pulando para esquerda
					curr_anim->reset();
					set_state(JUMPING);
					break;
				}
				else
				{
					vel.zero();
					acc.zero();
					
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[2]; // pulando ou caindo para direita
					else
						curr_anim = &anim[8]; // pulando ou caindo para esquerda
					curr_anim->reset();
					set_state(JUMPING);
					break;
				}
			}
			
			if (!left_key && !right_key && !up_key && !down_key)
			{
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[3]; // parado na árvore para direita
				else
					curr_anim = &anim[9]; // parado na árvore para esquerda
				curr_anim->reset();
				set_state(STANDING_TREE);
				break;
			}
			
			if (left_key)
			{
				dir = LEFT_PLAYER;
				if (vel.x > 0)
					vel.x = 0;
				acc.x = -config.acc.x;
				curr_anim = &anim[10]; // andando na árvore para esquerda
			}
			else if (right_key)
			{
				dir = RIGHT_PLAYER;
				if (vel.x < 0)
					vel.x = 0;
				acc.x = config.acc.x;
				curr_anim = &anim[4]; // andando na árvore para direita
			}
			else
			{
				acc.x = 0;
				vel.x = 0;
			}
			
			if (up_key)
			{
				if (vel.y > 0)
					vel.y = 0;
				acc.y = -config.acc_jetpack.y;
			}
			else if (down_key)
			{
				if (vel.y < 0)
					vel.y = 0;
				acc.y = config.acc_jetpack.y;
			}
			else
			{
				acc.y = 0;
				vel.y = 0;
			}
			
			vel.x += acc.x;
			if (vel.x > config.vel_max_tree)
				vel.x = config.vel_max_tree;
			else if (vel.x < -config.vel_max_tree)
				vel.x = -config.vel_max_tree;
			
			vel.y += acc.y;
			if (vel.y > config.vel_max_tree)
				vel.y = config.vel_max_tree;
			else if (vel.y < -config.vel_max_tree)
				vel.y = -config.vel_max_tree;
			
			collision_ver();
			pos.y += vel.y;
			collision_hor();
			pos.x += vel.x;
			
			// limita o movimento no eixo X
			if (pos.x + c_point[0].x > limit.x + limit.w - 2)
				pos.x = limit.x + limit.w - c_point[0].x - 2;
			else if (pos.x + c_point[2].x < limit.x)
				pos.x = limit.x - c_point[2].x;
			
			// coloca no meio do tronco em X
			if (map->get_tile(pos.x + c_point[8].x, pos.y + c_point[8].y) == 'i')
			{
				vel.x = 0;
				acc.x = 0;
				pos.x = (int(pos.x + c_point[8].x) / map->get_tilesize()) * map->get_tilesize();

				if ((left_key || right_key) && !up_key)
				{
					if (!ground())
					{
						if (map->get_tile(pos.x + c_point[8].x, pos.y + c_point[8].y) == 'i')
						{
							vel.y = -config.vel_max.y;
							collision_ver();
							pos.y += vel.y;
							if (dir == RIGHT_PLAYER)
								curr_anim = &anim[2]; // caindo ou pulando para direita
							else
								curr_anim = &anim[8]; // caindo ou pulando para esquerda
							curr_anim->reset();
							set_state(JUMPING);
							break;
						}
					}
					else
					{
						vel.zero();
						acc.zero();
						if (dir == RIGHT_PLAYER)
							curr_anim = &anim[0]; // parado para direita
						else
							curr_anim = &anim[6]; // parado para esquerda
						curr_anim->reset();
						set_state(STANDING);
						break;
					}
				}
			}

			process();
			
			if (touch_bad_tile() || shot_at || touched_alien)
			{
				kill();
				break;
			}

			curr_anim->update();
			break;
		
		case STANDING_FIRE:
			if (up_key)
			{
				if (tree())
				{
					pos.x = (int(pos.x + c_point[8].x) / map->get_tilesize()) * map->get_tilesize();

					acc.zero();
					vel.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[3]; // parado na árvore para direita
					else
						curr_anim = &anim[9]; // parado na árvore para esquerda
					curr_anim->reset();
					set_state(STANDING_TREE);
					break;
				}
				
				vel.y = -config.vel_max.y;
				collision_ver();
				pos.y += vel.y;
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[2]; // caindo ou pulando para direita
				else
					curr_anim = &anim[8]; // caindo ou pulando para esquerda
				curr_anim->reset();
				set_state(JUMPING);
				break;
			}
			
			if (jetpack_key)
				if (jetpack.has_jetpack() && jetpack.has_fuel())
				{
					vel.zero();
					acc.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[5]; // no jetpack para direita
					else
						curr_anim = &anim[11]; // no jetpack para esquerda
					set_state(JETPACK);
					break;
				}
		
		
			process();
			
			if (shot_at || touched_alien)
			{
				kill();
				break;
			}
		
			if (curr_anim->update() == 3)
			{
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[0]; // parado para direita
				else
					curr_anim = &anim[6]; // parado para esquerda
				curr_anim->reset();
				set_state(STANDING);
			}
			break;
			
		case JUMPING_FIRE:
			if (ground())
			{
				acc.zero();
				vel.zero();
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[0]; // parado para direita
				else
					curr_anim = &anim[6]; // parado para esquerda
				curr_anim->reset();
				set_state(STANDING);
				break;
			}
			
			if (up_key)
			{
				if (tree())
				{
					pos.x = (int(pos.x + c_point[8].x) / map->get_tilesize()) * map->get_tilesize();

					acc.zero();
					vel.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[3]; // parado na árvore para direita
					else
						curr_anim = &anim[9]; // parado na árvore para esquerda
					curr_anim->reset();
					set_state(STANDING_TREE);
					break;
				}
			}
			
			if (left_key)
			{
				acc.x = -config.acc.x;
				if (dir == RIGHT_PLAYER)
				{
					dir = LEFT_PLAYER;
					curr_anim = &anim[14]; // caindo ou pulando para esquerda
					curr_anim->set_index(anim[15].get_index());
				}
			}
			else if (right_key)
			{
				acc.x = config.acc.x;
				
				if (dir == LEFT_PLAYER)
				{
					dir = RIGHT_PLAYER;
					curr_anim = &anim[15]; // caindo ou pulando para esquerda
					curr_anim->set_index(anim[14].get_index());
				}
			}
			else
			{
				acc.x = 0;
			}
			
			if (jetpack_key)
				if (jetpack.has_jetpack() && jetpack.has_fuel())
				{
					vel.zero();
					acc.zero();
					if (dir == RIGHT_PLAYER)
						curr_anim = &anim[5]; // no jetpack para direita
					else
						curr_anim = &anim[11]; // no jetpack para esquerda
					set_state(JETPACK);
					break;
				}
			
			vel.x += acc.x;
			if (vel.x > config.vel_max.x)
				vel.x = config.vel_max.x;
			else if (vel.x < -config.vel_max.x)
				vel.x = -config.vel_max.x;

			vel.y += config.gravity;
			if (vel.y > config.vel_max.y)
				vel.y = config.vel_max.y;
			else if (vel.y < -config.vel_max.y)
				vel.y = -config.vel_max.y;
			
			collision_ver();
			pos.y += vel.y;
			collision_hor();
			pos.x += vel.x;
			
			// limita o movimento no eixo X
			if (pos.x + c_point[0].x > limit.x + limit.w - 2)
				pos.x = limit.x + limit.w - c_point[0].x - 2;
			else if (pos.x + c_point[2].x < limit.x)
				pos.x = limit.x - c_point[2].x;
			
			// limita o movimento no eixo Y
			if (pos.y > limit.y + limit.h) // se passou por baixo da tela
				pos.y = limit.y - map->get_tilesize(); // volte lá pra cima da fase
			
			process();
			
			if (touch_bad_tile() || shot_at || touched_alien)
			{
				kill();
				break;
			}
			
			collect_items();
			if (enter_door())
			{
				curr_anim = 0;
				gun.set_gun(false);
				jetpack.set_jetpack(false);
				set_state(FINISHED_LEVEL);
				break;
			}

			if (curr_anim->update() == 3)
			{
				if (dir == RIGHT_PLAYER)
					curr_anim = &anim[2]; // caindo ou pulando para direita
				else
					curr_anim = &anim[8]; // caindo ou pulando para esquerda
				
				set_state(JUMPING);
			}
			break;
		
		case DYING_PLAYER: // morrendo
			if (curr_anim->update() == 3)
			{
				lives--;
				if (lives < 0)
				{
					curr_anim = 0;
					kernel = false;
					gun.set_gun(false);
					jetpack.set_jetpack(false);
					set_state(GAMEOVER); // fim do jogo
					break;
				}

				// reinicia na fase
				curr_anim = &anim[13]; // esperando ser controlado
				curr_anim->reset();
				reset(); // reinicia na fase
			}
			break;
		
		case GAMEOVER: // game over
			if (jetpack.has_jetpack())
				jetpack.set_jetpack(false);
			if (gun.has_gun())
				gun.set_gun(false);
			break;
		
		case FINISHED_LEVEL: // passou de fase
			if (jetpack.has_jetpack())
				jetpack.set_jetpack(false);
			if (gun.has_gun())
				gun.set_gun(false);
			break;

		case WALKING_TRANSITION:
			if (ground())
			{
				dir = RIGHT_PLAYER;
				if (vel.x < 0)
					vel.x = 0;
				acc.x = config.acc.x;
				curr_anim = &anim[1]; // caminhando para direita
			}
			else
			{
				vel.y = 0;
				curr_anim = &anim[2]; // caindo ou pulando para direita
				curr_anim->reset();
				set_state(JUMPING_TRANSITION);
				break;
			}

			vel.x += acc.x;
			if (vel.x > config.vel_max.x)
				vel.x = config.vel_max.x;
			else if (vel.x < -config.vel_max.x)
				vel.x = -config.vel_max.x;
			
			collision_ver();
			pos.y += vel.y;
			collision_hor();
			pos.x += vel.x;
			
			// limita o movimento no eixo X
			if (pos.x + c_point[0].x > limit.x + limit.w - 2)
				pos.x = limit.x + limit.w - c_point[0].x - 2;
			else if (pos.x + c_point[2].x < limit.x)
				pos.x = limit.x - c_point[2].x;
			
			if (pos.x > final_pos + c_point[0].x)
			{
				curr_anim = 0;
				set_state(INACTIVE_PLAYER);
				break;
			}
			
			curr_anim->update();
			break;
		
		case JUMPING_TRANSITION:
			if (ground())
			{
				dir = RIGHT_PLAYER;
				if (vel.x < 0)
					vel.x = 0;
				acc.x = config.acc.x;
				curr_anim = &anim[1]; // caminhando para direita
				curr_anim->reset();
				set_state(WALKING_TRANSITION);
				break;
			}
			
			dir = RIGHT_PLAYER;
			acc.x = config.acc.x;
			curr_anim = &anim[2]; // caindo ou pulando para direita
			
			vel.x += acc.x;
			if (vel.x > config.vel_max.x)
				vel.x = config.vel_max.x;
			else if (vel.x < -config.vel_max.x)
				vel.x = -config.vel_max.x;

			vel.y += config.gravity;
			if (vel.y > config.vel_max.y)
				vel.y = config.vel_max.y;
			else if (vel.y < -config.vel_max.y)
				vel.y = -config.vel_max.y;
			
			collision_ver();
			pos.y += vel.y;
			collision_hor();
			pos.x += vel.x;
			
			// limita o movimento no eixo X
			if (pos.x + c_point[0].x > limit.x + limit.w - 2)
				pos.x = limit.x + limit.w - c_point[0].x - 2;
			else if (pos.x + c_point[2].x < limit.x)
				pos.x = limit.x - c_point[2].x;
			
			// limita o movimento no eixo Y
			if (pos.y > limit.y + limit.h) // se passou por baixo da tela
				pos.y = limit.y - map->get_tilesize(); // volte lá pra cima da fase

			if (pos.x > final_pos + c_point[0].x)
			{
				curr_anim = 0;
				set_state(INACTIVE_PLAYER);
				break;
			}

			curr_anim->update();
			break;
		
		case INACTIVE_PLAYER:
			break;
			
		default:
			set_state(WAITING);
			break;
	}
	
	return get_state();
}


