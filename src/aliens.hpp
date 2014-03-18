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
	SVect acc;
	float shot_vel;
	
	SAlienConfig (  )
	{
		vel_max.x = 3;
		vel_max.y = 14;
		acc.x = 1;
		acc.y = 1;
		shot_vel = 10;
	}
};

class CWalkerAlien: public CGameEntity
{
	protected:
		CPlayer * player;
		bool shot_at; // se foi baleado pelo jogador
		bool touched_player; // se tocou o jogador
		int dir; // direção da animação
		float move_distance; // distância de movimento, ou raio de movimento
		SVect init_pos;
		vector <int> coll_tiles;
		vector <SVect> c_point;
		CAnimation * curr_anim;
		vector <CAnimation> anim;
		CTileMap * map;

	public:		
		CGun gun;
		SAlienConfig config;
		
	public:
		#ifndef USE_SDL2
			CWalkerAlien ( CPlayer * p, int m_d, SVect i_p )
		#else
			CWalkerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, SVect i_p )
		#endif
		{
			#if USE_SDL2
				SDL_Texture * texture = 0;
			#endif
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
			SVect pd[2] = {SVect(0,20),SVect(38,20)};
			gun.set_pos_dir(pd);
			
			coll_tiles.push_back('a');
			coll_tiles.push_back('b');
			coll_tiles.push_back('c');
			coll_tiles.push_back('u');
			
			// pontos de colisão
			// colisão a direita
			c_point.push_back(SVect(31,6));
			c_point.push_back(SVect(31,43));
			// colisão a esquerda
			c_point.push_back(SVect(15,6));
			c_point.push_back(SVect(15,43));
			// colisão abaixo
			c_point.push_back(SVect(15,43));
			c_point.push_back(SVect(23,43));
			c_point.push_back(SVect(31,43));
			// colisão acima
			c_point.push_back(SVect(15,6));
			c_point.push_back(SVect(31,6));
			// centro de colisão
			c_point.push_back(SVect(23,25));
			// meio da lateral esquerda
			c_point.push_back(SVect(14,25));
			// meio da lateral direita
			c_point.push_back(SVect(32,25));

			anim.resize(6);

			#if _WIN32 || _WIN64 || __MINGW32__
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
			
			#ifndef USE_SDL2
				anim[0].surface = optimize_surface_alpha(IMG_Load(path));
				if (!anim[0].surface)
					throw "CWalkerAlien: não foi possível abrir walkeralien_right.png\n";
				
				// virado para direita
				// caminhando para direita
				anim[0].add_frame((SDL_Rect){0,0,48,48}, 3);
				anim[0].add_frame((SDL_Rect){48,0,48,48}, 5);
				anim[0].add_frame((SDL_Rect){48*2,0,48,48}, 3);
				// atirando para direita
				anim[1].add_frame((SDL_Rect){0,48,48,48}, 3);
				anim[1].add_frame((SDL_Rect){48,48,48,48}, 3);
				anim[1].add_frame((SDL_Rect){48*2,48,48,48}, 3);
				// morrendo
				anim[4].add_frame((SDL_Rect){0,48*2,48,48}, 6);
				anim[4].add_frame((SDL_Rect){48,48*2,48,48}, 6);
				anim[4].add_frame((SDL_Rect){48*2,48*2,48,48}, 6);
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "CWalkerAlien: não foi possível abrir walkeralien_right.png\n";
				
				// virado para direita
				// caminhando para direita
				anim[0].add_frame(texture, (SDL_Rect){0,0,48,48}, 3);
				anim[0].add_frame(texture, (SDL_Rect){48,0,48,48}, 5);
				anim[0].add_frame(texture, (SDL_Rect){48*2,0,48,48}, 3);
				// atirando para direita
				anim[1].add_frame(texture, (SDL_Rect){0,48,48,48}, 3);
				anim[1].add_frame(texture, (SDL_Rect){48,48,48,48}, 3);
				anim[1].add_frame(texture, (SDL_Rect){48*2,48,48,48}, 3);
				// morrendo
				anim[4].add_frame(texture, (SDL_Rect){0,48*2,48,48}, 6);
				anim[4].add_frame(texture, (SDL_Rect){48,48*2,48,48}, 6);
				anim[4].add_frame(texture, (SDL_Rect){48*2,48*2,48,48}, 6);
			#endif
			
			
			#if _WIN32 || _WIN64 || __MINGW32__
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
			
			#ifndef USE_SDL2
				anim[2].surface = optimize_surface_alpha(IMG_Load(path));
				if (!anim[2].surface)
					throw "CWalkerAlien: não foi possível abrir walkeralien_left.png\n";
				
				// virado para esquerda
				// caminhando para esquerda
				anim[2].add_frame((SDL_Rect){0,0,48,48}, 3);
				anim[2].add_frame((SDL_Rect){48,0,48,48}, 5);
				anim[2].add_frame((SDL_Rect){48*2,0,48,48}, 3);
				// atirano para esquerda
				anim[3].add_frame((SDL_Rect){0,48,48,48}, 3);
				anim[3].add_frame((SDL_Rect){48,48,48,48}, 3);
				anim[3].add_frame((SDL_Rect){48*2,48,48,48}, 3);
				//morrendo
				anim[5].add_frame((SDL_Rect){0,48*2,48,48}, 6);
				anim[5].add_frame((SDL_Rect){48,48*2,48,48}, 6);
				anim[5].add_frame((SDL_Rect){48*2,48*2,48,48}, 6);
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "CWalkerAlien: não foi possível abrir walkeralien_left.png\n";
				
				// virado para esquerda
				// caminhando para esquerda
				anim[2].add_frame(texture, (SDL_Rect){0,0,48,48}, 3);
				anim[2].add_frame(texture, (SDL_Rect){48,0,48,48}, 5);
				anim[2].add_frame(texture, (SDL_Rect){48*2,0,48,48}, 3);
				// atirano para esquerda
				anim[3].add_frame(texture, (SDL_Rect){0,48,48,48}, 3);
				anim[3].add_frame(texture, (SDL_Rect){48,48,48,48}, 3);
				anim[3].add_frame(texture, (SDL_Rect){48*2,48,48,48}, 3);
				//morrendo
				anim[5].add_frame(texture, (SDL_Rect){0,48*2,48,48}, 6);
				anim[5].add_frame(texture, (SDL_Rect){48,48*2,48,48}, 6);
				anim[5].add_frame(texture, (SDL_Rect){48*2,48*2,48,48}, 6);
			#endif

			curr_anim = &anim[0];
			
			#ifndef USE_SDL2
				anim[1].surface = anim[0].surface;
				anim[3].surface = anim[2].surface;
				anim[4].surface = anim[0].surface;
				anim[5].surface = anim[2].surface;
			#endif
			reset();
			
		}
		
		~CWalkerAlien (  )
		{
			#ifndef USE_SDL2
				if (anim[0].surface)
					SDL_FreeSurface(anim[0].surface);
			
				if (anim[2].surface)
					SDL_FreeSurface(anim[2].surface);
			#else
				anim[0].destroy_textures();
				anim[2].destroy_textures();
			#endif
		}
		
		void set_map ( CTileMap * m )
		{
			map = m;
			gun.shot.set_map(m);
		}
		
		void reset (  )
		{
			pos = init_pos;
			vel.y = 1;
			pos.y += c_point[5].y;
			collide_ver();
			set_state(STOPED_ALIEN);
		}
		
		int collide_hor (  )
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
		
		int collide_ver (  )
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
		
		bool has_coll_tile ( int tile )
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
		
		int no_ground (  )
		{
			if (!has_coll_tile(map->get_tile(pos.x + c_point[4].x, pos.y + c_point[4].y + 1)))
				return -1;
			
			if (!has_coll_tile(map->get_tile(pos.x + c_point[6].x, pos.y + c_point[6].y + 1)))
				return 1;

			return 0;
		}
		
		int wall (  )
		{
			if (has_coll_tile(map->get_tile(pos.x + c_point[10].x, pos.y + c_point[10].y))) // se colidiu a esquerda
				return -1;

			if (has_coll_tile(map->get_tile(pos.x + c_point[11].x, pos.y + c_point[11].y))) // se colidiu a direita
				return 1;

			return 0;
		}
		
		void kill (  )
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
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			#ifndef USE_SDL2
				if (curr_anim)
					curr_anim->draw(pos.x, pos.y, cam, screen);
			
				gun.draw(cam, screen);
			#else
				if (curr_anim)
					curr_anim->draw(pos.x, pos.y, cam, renderer);
			
				gun.draw(cam, renderer);
				#endif
			
			if (get_state() == INACTIVE_ALIEN)
				return;

			SDL_Rect a, b;
			
			a = cam->get_dimension();
			a.x = cam->get_position().x - map->get_tilesize();
			a.y = cam->get_position().y;
			a.w += map->get_tilesize() * 2;
			
			b = dim;
			b.x = pos.x + dim.x;
			b.y = pos.y + dim.y;

			if (!boudingbox(a, b) && !pointbox(init_pos + c_point[9], a))
			{
				reset();
			}
			else if (get_state() == STOPED_ALIEN)
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
		}
		
		void process (  )
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
		
		int update (  )
		{
			switch (get_state())
			{
				case STOPED_ALIEN:
					gun.update();
					break;
			
				case MOVING_ALIEN:
					if (no_ground() || (pos.x + c_point[9].x) - init_pos.x > move_distance ||
					   (pos.x + c_point[9].x) - init_pos.x < -move_distance ||
					   wall())
					{
						acc.y = config.acc.y;
						
						if (dir == LEFT_ALIEN)
						{
							dir = RIGHT_ALIEN;
							acc.x = config.acc.x;
							vel.x = -vel.x;
							curr_anim = &anim[0]; // caminhando para direita
							curr_anim->reset();
							
							while (no_ground() == -1 || (pos.x + c_point[9].x) - init_pos.x < -move_distance || wall() == -1)
							{
								pos.x += 1; // move para direita
							}
						}
						else
						{
							dir = LEFT_ALIEN;
							acc.x = -config.acc.x;
							vel.x = -vel.x;
							curr_anim = &anim[2]; // caminhando para esquerda
							curr_anim->reset();
							
							while (no_ground() == 1 || (pos.x + c_point[9].x) - init_pos.x > move_distance || wall() == 1)
							{
								pos.x -= 1; // move para esquerda
							}
						}
					}
					
					if (!gun.was_used())
					{
						int r = (75.0 * rand()) / (RAND_MAX + 1.0);
						if (r == 0)
							gun.fire(pos, dir, SVect((dir == LEFT_ALIEN)? -config.shot_vel: config.shot_vel, 0));
						
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

					vel.y += acc.y;
					if (vel.y > config.vel_max.y)
						vel.y = config.vel_max.y;
					else if (vel.y < -config.vel_max.y)
						vel.y = -config.vel_max.y;

					collide_hor();
					pos.x += vel.x;
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
};

class CFlyerAlien: public CGameEntity
{
	protected:
		int dir;
		int shot_dir;
		bool shot_at;
		bool touched_player;
		float move_distance; // distância de movimento, ou raio de movimento
		float count, step; // para mover o alien
		SVect init_pos;
		SVect c_point; // centro da animação
		CPlayer * player;
		CTileMap * map;
		CAnimation * curr_anim;
		vector <CAnimation> anim;
		

	public:
		CGun gun;
		SAlienConfig config;
		
	public:
		#ifndef USE_SDL2
			CFlyerAlien ( CPlayer * p, int m_d, SVect i_p )
		#else
			CFlyerAlien ( SDL_Renderer * r, CPlayer * p, int m_d, SVect i_p )
		#endif
		{
			#if USE_SDL2
				SDL_Texture * texture = 0;
			#endif
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
			
			#if _WIN32 || _WIN64 || __MINGW32__
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
			
			#ifndef USE_SDL2
				anim[0].surface = optimize_surface_alpha(IMG_Load(path));
				if (!anim[0].surface)
					throw "CFlyerAlien: não conseguiu abrir flyeralien.png\n";
				
				// animações
				// voando
				anim[0].add_frame((SDL_Rect){0,0,48,48}, 3);
				// atirando
				anim[1].add_frame((SDL_Rect){0,0,48,48}, 3);
				// morrendo
				anim[2].add_frame((SDL_Rect){0,48*2,48,48}, 6);
				anim[2].add_frame((SDL_Rect){48,48*2,48,48}, 6);
				anim[2].add_frame((SDL_Rect){48*2,48*2,48,48}, 6);
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
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
			#endif
			
			#ifndef USE_SDL2
				anim[1].surface = anim[2].surface = anim[0].surface;
			#endif
			reset();
		}
		
		~CFlyerAlien (  )
		{
			#ifndef USE_SDL2
				if (anim[0].surface)
					SDL_FreeSurface(anim[0].surface);
			#else
				anim[0].destroy_textures();
			#endif
		}
		void set_map ( CTileMap * m )
		{
			map = m;
			gun.shot.set_map(m);
		}
		
		void reset (  )
		{
			count = -1.57;
			step = 0.05;
			pos.x = init_pos.x + move_distance * cos(count);
			pos.y = init_pos.y + move_distance * sin(count);
			curr_anim = &anim[0]; // alien parado
			set_state(STOPED_ALIEN);
		}
		
		void kill (  )
		{
			acc.zero();
			vel.zero();
			curr_anim = &anim[2]; // morrendo
			curr_anim->reset();
			set_state(DYING_ALIEN);
		}
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen )
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer )
		#endif
		{
			#ifndef USE_SDL2
				if (curr_anim)
					curr_anim->draw(pos.x, pos.y, cam, screen);
			
				gun.draw(cam, screen);
			#else
				if (curr_anim)
					curr_anim->draw(pos.x, pos.y, cam, renderer);
				
				gun.draw(cam, renderer);
			#endif
			
			if (get_state() == INACTIVE_ALIEN)
				return;
			
			SDL_Rect a, b;
			
			a = cam->get_dimension();
			a.x = cam->get_position().x - map->get_tilesize();
			a.y = cam->get_position().y;
			a.w += map->get_tilesize() * 2;
			
			b = dim;
			b.x = pos.x + dim.x;
			b.y = pos.y + dim.y;

			if (!boudingbox(a, b) && !pointbox(init_pos + c_point, a))
			{
				reset();
			}
			else if (get_state() == STOPED_ALIEN)
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
		}
		
		void process (  )
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
		
		int update (  )
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
							gun.fire(pos, shot_dir, SVect((shot_dir == LEFT_ALIEN)? -config.shot_vel: config.shot_vel, 0));
						
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
};


class CCircleAlien: public CGameEntity
{
	protected:
		int dir;
		bool touched_player;
		SVect init_pos;
		SVect c_point; // centro da animação
		CPlayer * player;
		CTileMap * map;
		CAnimation * curr_anim;
		vector <CAnimation> anim;
	
	public:
		#ifndef USE_SDL2
			CCircleAlien ( CPlayer * p, SVect i_p )
		#else
			CCircleAlien ( SDL_Renderer * r, CPlayer * p, SVect i_p )
		#endif
		{
			dir = LEFT_ALIEN;
			init_pos = i_p;
			player = p;
			map = player->get_map();
			
		}
};


#endif

