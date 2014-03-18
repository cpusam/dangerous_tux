#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

class CLabelCamera: public CLabel
{
	public:
		CLabelCamera ( string s, SDL_Color c): CLabel(s,c)
		{
		}
		
		#ifndef USE_SDL2
			void draw ( CCamera * cam, SDL_Surface * screen )
			{
				if (visible)
					draw_surface(get_surface(), pos.x, pos.y, cam, screen);
			}
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer )
			{
				if (visible)
					draw_texture(get_texture(), pos.x, pos.y, cam, renderer);
			}
		#endif
};

enum CGameMenuState
{
	INIT_GAMEMENU,
	RUNNING_GAMEMENU,
	END_GAMEMENU
};

class CGameMenu: public CStateMachine
{
	protected:
		CPlayer * player;
		CCamera * cam;
		CTileMapView * map;
		CBackground * bg;
		CLabelCamera * controls;
		CLabelCamera * climbtree;
		CLabelCamera * tools;
		SVect player_pos;
		#ifndef USE_SDL2
			SDL_Surface * screen;
		#else
			SDL_Renderer * renderer;
		#endif
		
	public:
		#ifndef USE_SDL2
			CGameMenu ( SDL_Surface * s, CPlayer * p, int ts )
		#else
			CGameMenu ( SDL_Renderer * r, CPlayer * p, int ts )
		#endif
		{
			map = new CTileMapView(ts);
			cam = new CCamera((SDL_Rect){0,0,ts * 20,ts * 13}, (SDL_Rect){0,0,0,0});
			player = p;
			bg = 0;

			#if _WIN32 || _WIN64 || __MINGW32__
				char path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
				#ifndef PREFIX
					sprintf(path, "%s\\fonts\\inhouseedition.ttf", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\fonts\\inhouseedition.ttf", PREFIX);
				#endif
			#else
				char path[1024];
				#ifndef PREFIX
					sprintf(path, "./fonts/inhouseedition.ttf");
				#else
					sprintf(path, "%s/share/games/dangeroustux/fonts/inhouseedition.ttf", PREFIX);
				#endif
			#endif
			// define a fonte para CWriter
			if (CWriter::instance()->set_font(path, 50) == 0)
				throw "CGameMenu: Não conseguiu abrir fonte\n";
			
			controls = new CLabelCamera("CONTROLS:\n-LEFT ARROW KEY TO MOVE LEFT\n-RIGHT ARROW KEY TO MOVE RIGHT\n-UP ARROW KEY TO JUMP", (SDL_Color){0,0,0,0});
			climbtree = new CLabelCamera("TOUCH THE TREE AND\nPRESS 'UP' KEY TO CLIMB IT", (SDL_Color){0,0,0,0});
			tools = new CLabelCamera("-PRESS 'A' KEY TO TURN ON/OFF THE JETPACK\n-PRESS 'S' TO USE LASER GUN", (SDL_Color){0,0,0,0});
			
			#ifndef USE_SDL2
				screen = s;
			#else
				renderer = r;
			#endif
			set_state(INIT_GAMEMENU);
		}
		
		~CGameMenu (  )
		{
			if (cam)
				delete cam;
		
			if (map)
				delete map;
			
			if (bg)
				delete bg;
			
			if (controls)
				delete controls;
			
			if (climbtree)
				delete climbtree;
			
			if (tools)
				delete tools;
		}
		
		void input ( SDL_Event & event )
		{
			player->input(event);
		}
		
		void reset (  )
		{
			if (get_state() != INIT_GAMEMENU)
			{
				player->set_map(map);
				player->set_respawn(player_pos);
				player->reset_gamemenu();
				set_state(RUNNING_GAMEMENU);
			}
		}
		
		#ifndef USE_SDL2
			void draw (  )
			{
				if (get_state() == INIT_GAMEMENU)
					return;

				bg->draw(cam, screen);
				map->draw(cam, screen);
				player->gun.draw(cam, screen);
				player->draw(cam, screen);
				
				if (controls->is_visible())
					controls->draw(cam, screen);
				else if (climbtree->is_visible())
					climbtree->draw(cam, screen);
				else if (tools->is_visible())
					tools->draw(cam, screen);
			}
		#else
			void draw (  )
			{
				if (get_state() == INIT_GAMEMENU)
					return;
				
				bg->draw(cam, renderer);
				map->draw(cam, renderer);
				player->gun.draw(cam, renderer);
				player->draw(cam, renderer);
				
				if (controls->is_visible())
					controls->draw(cam, renderer);
				else if (climbtree->is_visible())
					climbtree->draw(cam, renderer);
				else if (tools->is_visible())
					tools->draw(cam, renderer);
			}
		#endif
		
		
		int update (  )
		{
			switch (get_state())
			{
				case INIT_GAMEMENU:
				{
					SDL_Surface * aux = 0;
			
					#if _WIN32 || _WIN64 || __MINGW32__
						char path[FILENAME_MAX];
						char pimage[FILENAME_MAX];
						char p2[FILENAME_MAX];
						_getcwd(p2, sizeof(p2));
						#ifndef PREFIX
							sprintf(path, "%s\\levels\\menumap.txt", p2);
							sprintf(pimage, "%s\\images\\", p2);
						#else
							sprintf(path, "%s\\dangeroustux\\levels\\menumap.txt", PREFIX);
							sprintf(pimage, "%s\\dangeroustux\\images\\", PREFIX);
						#endif
					#else
						char path[1024];
						char pimage[1024];
						#ifndef PREFIX
							sprintf(path, "./levels/menumap.txt");
							sprintf(pimage, "./images/");
						#else
							sprintf(path, "%s/share/games/dangeroustux/levels/menumap.txt", PREFIX);
							sprintf(pimage, "%s/share/games/dangeroustux/images/", PREFIX);
						#endif
					#endif

					string nimage; // nome da imagem
					ifstream file(path, ifstream::binary);
					if (!file)
						throw "CGameMenu: Não foi possível ler o arquivo de level\n";
					getline(file, nimage);
					sprintf(path, "%s%s", pimage, nimage.c_str());

					if (!bg)
					{
						#ifndef USE_SDL2
							bg = new CBackground(optimize_surface_alpha(IMG_Load(path)));
							if (!bg->get_surface())
							{
								sprintf(path, "CGameMenu: não foi possível abrir background %s%s\n", pimage, nimage.c_str());
								throw path;
							}
						#else
							aux = IMG_Load(path);
							bg = new CBackground(SDL_CreateTextureFromSurface(renderer, aux));
							SDL_FreeSurface(aux);
							if (!bg->get_texture())
							{
								sprintf(path, "CGameMenu: não foi possível abrir background %s%s\n", pimage, nimage.c_str());
								throw path;
							}
						#endif
					}

					vector <int> tileset;
					string line;
					int i;
		
					for (i = 0; i < 13; i++)
					{
						if (!file.eof())
						{
							line = "";
							getline(file, line);
							int j;

							for (j = 0; j < line.size(); j++)
									tileset.push_back(line[j]);

							if (line[j] == '\0')
							{
								if (line[j - 1] == '\r')
									tileset[tileset.size() - 1] = -1;
								else
									tileset.push_back(-1);
							}
						}
						else
							break;
					}
					file.close();

					if (!map->read(tileset))
						throw "CGameMenu: não foi possível iniciar o mapa\n";
					
					if (map->get_width() < 20)
						throw "CGameMenu: falha ao ler o mapa, lido menos de 20 colunas\n";
					
					if (map->get_height() < 12)
						throw "CGameMenu: falha ao ler o mapa, lido menos de 13 linhas\n";

					cam->set_limit(map->get_dimension());
					player->set_map(map);
		
					// verifica se já não foi carregado a imagem dos tiles
					#ifndef USE_SDL2
						if (!map->surface)
					#else
						if (!map->texture)
					#endif
					{
						#if _WIN32 || _WIN64 || __MINGW32__
							#ifndef PREFIX
								sprintf(path, "%s\\images\\tiles.png", p2);
							#else
								sprintf(path, "%s\\dangeroustux\\images\\tiles.png", PREFIX);
							#endif
						#else
							#ifndef PREFIX
								sprintf(path, "./images/tiles.png");
							#else
								sprintf(path, "%s/share/games/dangeroustux/images/tiles.png", PREFIX);
							#endif
						#endif
			
						#ifndef USE_SDL2
							map->surface = optimize_surface_alpha(IMG_Load(path));
							if (!map->surface)
								throw SDL_GetError();
						#else
							aux = IMG_Load(path);
							map->texture = SDL_CreateTextureFromSurface(renderer, aux);
							SDL_FreeSurface(aux);
							if (!map->texture)
								throw SDL_GetError();
						#endif
					}
			
					// procura o caractere P no mapa e reinicia o player
					int tile, w, h;
					bool has_player = false, has_exit = false;
					SVect p;
					SDL_Rect d = map->get_dimension();
					
					for (i = 0; (tile = map->get_tile(i)) >= 0; i++)
					{
						if (tile == 'P')
						{
							// se é o tile de respawn do jogador, então defina o respawn
							p.x = (i % map->get_width()) * map->get_tilesize();
							p.y = (i / map->get_width()) * map->get_tilesize();
							player_pos = p;
							player->set_respawn(p);
							player->reset_gamemenu();
							player->set_kernel(true);
							player->gun.shot.clear_targets();
							player->gun.set_gun(false);
							player->jetpack.set_jetpack(false);
							has_player = true;
						}
						else if (tile == 'E')
						{
							has_exit = true;
						}
						else if (tile == 'A')
						{
							#ifndef USE_SDL2
								w = controls->get_surface()->w;
								h = controls->get_surface()->h;
							#else
								w = controls->get_texture_width();
								h = controls->get_texture_height();
							#endif
							p.x = (i % map->get_width()) * map->get_tilesize() - w/2;
							p.y = ((i / map->get_width())) * map->get_tilesize() - h;
							
							if (p.x < d.x)
								p.x = d.x;
							else if (p.x + w > d.x + d.w)
								p.x = d.x + d.w - w;
							
							if (p.y < d.y)
								p.y = d.y;
							else if (p.y + h > d.y + d.h)
								p.y = d.y + d.h - h;

							controls->set_pos(p);
						}
						else if (tile == 'B')
						{
							#ifndef USE_SDL2
								w = climbtree->get_surface()->w;
								h = climbtree->get_surface()->h;
							#else
								w = climbtree->get_texture_width();
								h = climbtree->get_texture_height();
							#endif
							p.x = (i % map->get_width()) * map->get_tilesize() - w/2;
							p.y = ((i / map->get_width())) * map->get_tilesize() - h;
							
							if (p.x < d.x)
								p.x = d.x;
							else if (p.x + w > d.x + d.w)
								p.x = d.x + d.w - w;
							
							if (p.y < d.y)
								p.y = d.y;
							else if (p.y + h > d.y + d.h)
								p.y = d.y + d.h - h;
							
							climbtree->set_pos(p);
						}
						else if (tile == 'C')
						{
							#ifndef USE_SDL2
								w = tools->get_surface()->w;
								h = tools->get_surface()->h;
							#else
								w = tools->get_texture_width();
								h = tools->get_texture_height();
							#endif
							p.x = (i % map->get_width()) * map->get_tilesize() - w/2;
							p.y = ((i / map->get_width())) * map->get_tilesize() - h;
							
							if (p.x < d.x)
								p.x = d.x;
							else if (p.x + w > d.x + d.w)
								p.x = d.x + d.w - w;
							
							if (p.y < d.y)
								p.y = d.y;
							else if (p.y + h > d.y + d.h)
								p.y = d.y + d.h - h;
							
							tools->set_pos(p);
						}
					}
			
					if (!has_player)
						throw "CGameMenu: player NÃO está no mapa, cadê o caractere P?\n";
					
					if (!has_exit)
						throw "CGameMenu: exit NÃO está no mapa, cadê o caractere E?\n";
					
					int ts = map->get_tilesize();
					map->remove_tile('.'); // remove o tile vazio da visão
					map->remove_tile('P'); // remove o jogador da visão
					map->remove_tile('K'); // remove o kernel da visão
					map->set_source('A', (SDL_Rect){ts*8,0,ts,ts}); // controles
					map->set_source('B', (SDL_Rect){ts*8,0,ts,ts}); // 
					map->set_source('C', (SDL_Rect){ts*8,0,ts,ts});
					map->set_source('a', (SDL_Rect){0,0,ts,ts});
					map->set_source('b', (SDL_Rect){ts,0,ts,ts});
					map->set_source('c', (SDL_Rect){ts*2,0,ts,ts});
					map->set_source('d', (SDL_Rect){ts*3,0,ts,ts});
					map->set_source('e', (SDL_Rect){ts*4,0,ts,ts});
					map->set_source('f', (SDL_Rect){ts*5,0,ts,ts});
					map->set_source('g', (SDL_Rect){ts*6,0,ts,ts});
					map->set_source('E', (SDL_Rect){ts*7,0,ts,ts});
					map->set_source('i', (SDL_Rect){0,ts,ts,ts});
					map->set_source('j', (SDL_Rect){ts,ts,ts,ts});
					map->set_source('k', (SDL_Rect){ts*2,ts,ts,ts});
					map->set_source('l', (SDL_Rect){ts*3,ts,ts,ts});
					map->set_source('m', (SDL_Rect){ts*4,ts,ts,ts});
					map->set_source('n', (SDL_Rect){ts*5,ts,ts,ts});
					map->set_source('s', (SDL_Rect){ts*6,ts*2,ts,ts});
					map->set_source('t', (SDL_Rect){ts*7,ts*2,ts,ts});
					map->set_source('u', (SDL_Rect){ts*6, ts*2,ts,ts});
					
					CAnimatedTile a;
					#ifndef USE_SDL2
						a.add_frame((SDL_Rect){ts*8,ts,ts,ts}, 4); // início
						a.add_frame((SDL_Rect){ts*9,ts,ts,ts}, 4);
						a.add_frame((SDL_Rect){ts*6,ts,ts,ts}, 4); // centro
						a.add_frame((SDL_Rect){ts*10,ts,ts,ts}, 4);
						a.add_frame((SDL_Rect){ts*11,ts,ts,ts}, 4); // final
						a.add_frame((SDL_Rect){ts*10,ts,ts,ts}, 4);
						a.add_frame((SDL_Rect){ts*6,ts,ts,ts}, 4); // centro
						a.add_frame((SDL_Rect){ts*9,ts,ts,ts}, 4);
					#else
						a.add_frame(map->texture, (SDL_Rect){ts*8,ts,ts,ts}, 4); // início
						a.add_frame(map->texture, (SDL_Rect){ts*9,ts,ts,ts}, 4);
						a.add_frame(map->texture, (SDL_Rect){ts*6,ts,ts,ts}, 4); // centro
						a.add_frame(map->texture, (SDL_Rect){ts*10,ts,ts,ts}, 4);
						a.add_frame(map->texture, (SDL_Rect){ts*11,ts,ts,ts}, 4); // final
						a.add_frame(map->texture, (SDL_Rect){ts*10,ts,ts,ts}, 4);
						a.add_frame(map->texture, (SDL_Rect){ts*6,ts,ts,ts}, 4); // centro
						a.add_frame(map->texture, (SDL_Rect){ts*9,ts,ts,ts}, 4);
					#endif
					map->add_animation(a, 'q'); // jetpack

					a.clear_frames();
					#ifndef USE_SDL2
						a.add_frame((SDL_Rect){ts*8,ts*2,ts,ts}, 4); // início
						a.add_frame((SDL_Rect){ts*9,ts*2,ts,ts}, 4);
						a.add_frame((SDL_Rect){ts*7,ts,ts,ts}, 4); // centro
						a.add_frame((SDL_Rect){ts*10,ts*2,ts,ts}, 4);
						a.add_frame((SDL_Rect){ts*11,ts*2,ts,ts}, 4); // final
						a.add_frame((SDL_Rect){ts*10,ts*2,ts,ts}, 4);
						a.add_frame((SDL_Rect){ts*7,ts,ts,ts}, 4); // centro
						a.add_frame((SDL_Rect){ts*9,ts*2,ts,ts}, 4);
					#else
						a.add_frame(map->texture, (SDL_Rect){ts*8,ts*2,ts,ts}, 4); // início
						a.add_frame(map->texture, (SDL_Rect){ts*9,ts*2,ts,ts}, 4);
						a.add_frame(map->texture, (SDL_Rect){ts*7,ts,ts,ts}, 4); // centro
						a.add_frame(map->texture, (SDL_Rect){ts*10,ts*2,ts,ts}, 4);
						a.add_frame(map->texture, (SDL_Rect){ts*11,ts*2,ts,ts}, 4); // final
						a.add_frame(map->texture, (SDL_Rect){ts*10,ts*2,ts,ts}, 4);
						a.add_frame(map->texture, (SDL_Rect){ts*7,ts,ts,ts}, 4); // centro
						a.add_frame(map->texture, (SDL_Rect){ts*9,ts*2,ts,ts}, 4);
					#endif
					map->add_animation(a, 'r');

					set_state(RUNNING_GAMEMENU);
					break;
				}
				
				case RUNNING_GAMEMENU:
					if (player->get_state() != FINISHED_LEVEL)
					{
						cam->lookat(player->get_pos());
						player->update();
						player->gun.update();
						player->jetpack.reload();
						map->update_animation();
						
						SVect p;
						p = player->get_center() + player->get_pos();
						
						if (pointbox(p, map->get_dimension()))
						{
							switch (map->get_tile(p.x, p.y))
							{
								case 'A':
									controls->show(true);
									break;
								
								case 'B':
									climbtree->show(true);
									break;
								
								case 'C':
									tools->show(true);
									break;
									
								default:
									controls->show(false);
									climbtree->show(false);
									tools->show(false);
									break;
							}
						}
					}
					else
						set_state(END_GAMEMENU);
					break;

				case END_GAMEMENU:
					break;

				default:
					set_state(INIT_GAMEMENU);
					break;
			}
			
			return get_state();
		}
};

#endif
