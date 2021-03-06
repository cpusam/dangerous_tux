#include "gun.hpp"

CShot::CShot (  )
{
	dim.x = dim.y = 0;
	dim.w = 20;
	dim.h = 5;
	map = 0;
	
	c_point.x = dim.w/2.0;
	c_point.y = dim.h/2.0;
	
	coll_tiles.push_back('a');
	coll_tiles.push_back('b');
	coll_tiles.push_back('c');
	coll_tiles.push_back('u');
	coll_tiles.push_back('s');
	coll_tiles.push_back('v');
	coll_tiles.push_back('w');
}

CShot::~CShot (  )
{
}

void CShot::set_map ( TileMap * m )
{
	map = m;
}

void CShot::set_shot ( Vect p, Vect v )// posição e direção
{
	pos = p;
	vel = v;
}

void CShot::add_target ( CGameEntity * t )
{
	if (t)
		target.push_back(t);
}

void CShot::rem_target ( CGameEntity * t )
{
	if (t)
		for (std::vector <CGameEntity *>::iterator it = target.begin(); it != target.end(); it++)
			if ((*it) == t)
			{
				(*it) = 0;
				break;
			}
}

void CShot::clear_targets (  )
{
	target.clear();
}

int CShot::collision (  )
{
	for (std::vector <int>::iterator i = coll_tiles.begin(); i != coll_tiles.end(); i++)
		if (map->get_tile(pos.x + c_point.x + vel.x, pos.y + c_point.y + vel.y) == *i)
			return 1;
	
	return 0;
}

int CShot::update (  )
{
	switch (get_state())
	{
		case 0:
			break;
		case 1:
			if (collision())
			{
				set_state(0);
				break;
			}

			pos += vel;
			
			for (std::vector <CGameEntity *>::iterator i = target.begin(); i != target.end(); i++)
			{
				if ((*i) == 0)
					continue;

				SDL_Rect a, b;
				a = get_dim();
				a.x += pos.x;
				a.y += pos.y;
				
				b = (*i)->get_dim();
				b.x += (*i)->get_pos().x;
				b.y += (*i)->get_pos().y;
				
				if (boundingbox(a, b))
				{
					SGameEvent e;
					e.receiver = (*i);
					e.event = "shot_at";
					if (id == "player_shot")
						rem_target((*i)); // remove o alien dos alvos
					CGameEventManager::instance()->add_event(e);
					set_state(0);
				}
			}
			break;
		default:
			break;
	}

	return get_state();
}

void CShot::draw ( Camera * cam, SDL_Renderer * renderer )
{
	if (get_state() != 1)
		return;

	SDL_Rect d;
	d.x = int(pos.x);
	d.y = int(pos.y);
	d.w = dim.w;
	d.h = dim.h;
	
	fill_rect(renderer, cam, (SDL_Color){255, 0, 0, 0}, d);
	
	Vect p = pos + c_point, cam_p = cam->get_position();
	SDL_Rect cam_d = cam->get_dimension();
	
	cam_d.x = cam_p.x;
	cam_d.y = cam_p.y;
	
	if (!pointbox(p, cam_d))
		set_state(0);
}


///////////////////////////////////////////////////////////////

CGun::CGun ( bool h )
{
	has = h;
	used = false;
	
	static int loaded = 0;
	if (!loaded)
	{
		SoundFX sound;
		#if _WIN32 || _WIN64
			char path[FILENAME_MAX];
			char p2[FILENAME_MAX];
			_getcwd(p2, sizeof(p2));
			#ifndef PREFIX
				sprintf(path, "%s\\sounds\\beam_fire.wav", p2);
			#else
				sprintf(path, "%s\\dangeroustux\\sounds\\beam_fire.wav", PREFIX);
			#endif
		#else
			char path[1024];
			#ifndef PREFIX
				sprintf(path, "./sounds/beam_fire.wav");
			#else
				sprintf(path, "%s/share/games/dangeroustux/sounds/beam_fire.wav", PREFIX);
			#endif
		#endif
		/*
		sound.set_chunk(path);
		Mix_VolumeChunk(sound.get_chunk(), 64);
		SoundPlayer::instance()->add_sound(sound);
		*/
		loaded = 1;
	}
}

CGun::CGun ( Vect p[2], bool h )
{
	has = h;
	used = false;
	pos_dir[0] = p[0];
	pos_dir[1] = p[1];
}

CGun::~CGun (  )
{
}

void CGun::set_pos_dir ( Vect p[2] )
{
	pos_dir[0] = p[0];
	pos_dir[1] = p[1];
}

void CGun::set_gun ( bool h )
{
	used = false;
	has = h;
}

bool CGun::has_gun (  )
{
	return has;
}

bool CGun::was_used (  )
{
	return used;
}

void CGun::fire ( Vect entity_pos, int d, Vect vel_shot )
{
	if ((d != 0 || d != 1) && used == true)
		return;
	
	used = true;

	shot.set_shot(entity_pos + pos_dir[d], vel_shot);
	SoundPlayer::instance()->play_sound("beam_fire.wav");
}

int CGun::update (  )
{
	switch (get_state())
	{
		case 0:
			if (used)
			{
				shot.set_state(1);
				set_state(1);
			}
			break;
		case 1:
			if (used)
			{
				if (shot.update() == 0)
				{
					used = false;
					set_state(0);
				}
			}
			else
			{
				set_state(0);
			}
			break;
		default:
			break;
	}
	
	return get_state();
}

void CGun::draw ( Camera * cam, SDL_Renderer * renderer )
{
	if (!used)
		return;
	shot.draw(cam, renderer);
}



