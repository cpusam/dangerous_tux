#ifndef GUN_HPP
#define GUN_HPP

#include "gameentity.hpp"

enum CShotType
{
	VER_SHOT, // tiro vertical
	HOR_SHOT, // tiro horizontal
	DIA_SHOT // tiro diagonal
};

class CShot: public CGameEntity
{
	protected:
		int dir; // direção a seguir
		vector <int> coll_tiles; // tiles de colisão
		SVect c_point; // ponto de colisão
		CTileMap * map; // mapa para colisão
		vector <CGameEntity *> target;

	public:
		CShot (  )
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
		}

		void set_map ( CTileMap * m )
		{
			map = m;
		}

		void set_shot ( SVect p, SVect v )// posição e direção
		{
			pos = p;
			vel = v;
		}
		
		void add_target ( CGameEntity * t )
		{
			if (t)
				target.push_back(t);
		}
		
		void rem_target ( CGameEntity * t )
		{
			if (t)
				for (vector <CGameEntity *>::iterator it = target.begin(); it != target.end(); it++)
					if ((*it) == t)
					{
						(*it) = 0;
						break;
					}
		}
		
		void clear_targets (  )
		{
			target.clear();
		}
		
		int collision (  )
		{
			for (vector <int>::iterator i = coll_tiles.begin(); i != coll_tiles.end(); i++)
				if (map->get_tile(pos.x + c_point.x + vel.x, pos.y + c_point.y + vel.y) == *i)
					return 1;
			
			return 0;
		}
		
		int update (  )
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
					
					for (vector <CGameEntity *>::iterator i = target.begin(); i != target.end(); i++)
					{
						if (!(*i))
							continue;

						SDL_Rect a, b;
						a = get_dim();
						a.x += pos.x;
						a.y += pos.y;
						
						b = (*i)->get_dim();
						b.x += (*i)->get_pos().x;
						b.y += (*i)->get_pos().y;
						
						if (boudingbox(a, b))
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
		
		void draw ( CCamera * cam, SDL_Surface * screen )
		{
			if (get_state() != 1)
				return;

			SDL_Rect d;
			d.x = int(pos.x);
			d.y = int(pos.y);
			d.w = dim.w;
			d.h = dim.h;
			
			fill_rect(cam, screen, SDL_MapRGB(screen->format, 255,0,0), d);
			
			SVect p = pos + c_point, cam_p = cam->get_position();
			SDL_Rect cam_d = cam->get_dimension();
			
			cam_d.x = cam_p.x;
			cam_d.y = cam_p.y;
			
			if (!pointbox(p, cam_d))
			{
				set_state(0);
			}
		}
};

class CGun: public CStateMachine
{
	protected:
		bool has; // se tem a arma
		bool used; // se foi usada e o tiro ainda está sendo visto
		SVect pos_dir[2]; // posição relativa do jogador para a arma
	public:
		CShot shot;
	
	public:
		CGun ( bool h=false )
		{
			has = h;
			used = false;
		}

		CGun ( SVect p[2], bool h=false )
		{
			has = h;
			used = false;
			pos_dir[0] = p[0];
			pos_dir[1] = p[1];
		}
		
		void set_pos_dir ( SVect p[2] )
		{
			pos_dir[0] = p[0];
			pos_dir[1] = p[1];
		}
		
		void set_gun ( bool h )
		{
			used = false;
			has = h;
		}

		bool has_gun (  )
		{
			return has;
		}
		
		bool was_used (  )
		{
			return used;
		}
		
		void fire ( SVect player_pos, int d, SVect vel_shot )
		{
			if ((d != 0 || d != 1) && used == true)
				return;
			
			used = true;
			shot.set_shot(player_pos + pos_dir[d], vel_shot);
		}
		
		int update (  )
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
		
		void draw ( CCamera * cam, SDL_Surface * screen )
		{
			if (!used)
				return;

			shot.draw(cam, screen);
		}
};

#endif

