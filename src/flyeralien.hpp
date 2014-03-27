#ifndef FLYERALIEN_HPP
#define FLYERALIEN_HPP

#include "aliensdef.hpp"

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

#endif

