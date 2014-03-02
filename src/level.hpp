#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <sstream>
#include <string.h>

#include "gameentity.hpp"
#include "player.hpp"
#include "aliens.hpp"
#include "gamesignal.hpp"

enum ELevelState
{
	INIT,
	RUNNING,
	NEXT_LEVEL
};

class CLevel: public CStateMachine
{
	private:
		int id;
		CTileMapView * map;
		CBackground * bg;
		string bg_path;

	public:
		vector <CGameEntity *> aliens;
		CKernelSignal kernel_signal;
		CExitSignal exit_signal;
		static CWidget * widget; // os widgets do level
		static CPlayer * player;
		static SDL_Surface * screen;
		static CCamera * cam;

	public:
		CLevel ( int tilesize, int i )
		{
			if (!widget)
				widget = new CWidget("level_window");

			id = i;
			map = new CTileMapView(tilesize);
			bg = 0;
		}
		
		~CLevel (  )
		{
			unload();
			delete_widget();
		}
		
		void delete_widget (  )
		{
			if (widget)
			{
				CWidget * w = widget->get_child(0);
				for (int i(0); w; i++, w = widget->get_child(i))
					delete w;
			
				delete widget;
				widget = 0;
			}
		}
		
		void unload (  )
		{
			if (map)
			{
				delete map;
				map = 0;
			}
			
			if (bg)
			{
				delete bg;
				bg = 0;
			}
			
			for (int i(0); i < aliens.size(); i++)
				if (aliens[i])
				{
					delete aliens[i];
					aliens[i] = 0;
				}
			
			aliens.clear();
		}
		
		string get_bg_path (  )
		{
			return bg_path;
		}
		
		CTileMapView * get_map (  )
		{
			return map;
		}
		
		int get_id (  )
		{
			return id;
		}

		void draw (  )
		{
			if (bg)
				bg->draw(cam, screen);
			else
			{
				SDL_Rect r = cam->get_dimension();
				SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 0xb6,0xb6,0xb6));
			}

			map->draw(cam, screen);
			player->gun.draw(cam, screen);
			player->draw(cam, screen);
			kernel_signal.draw(screen);
			exit_signal.draw(screen);
			
			for (vector <CGameEntity *>::iterator i = aliens.begin(); i != aliens.end(); i++)
				(*i)->draw(cam, screen);
			
			if (widget)
				widget->draw(screen);
		}
	
		void input ( SDL_Event & event )
		{
			widget->input(event);
			player->input(event);
		}
	
		int update (  )
		{
			switch (get_state())
			{
				case INIT: // inicializando
				{
					#if _WIN32 || _WIN64 || __MINGW32__
						char path[FILENAME_MAX];
						char pimage[FILENAME_MAX];
						char p2[FILENAME_MAX];
						_getcwd(p2, sizeof(p2));
						#ifndef PREFIX
							sprintf(path, "%s\\levels\\level%d.txt", p2, id);
							sprintf(pimage, "%s\\images\\", p2);
						#else
							sprintf(path, "%s\\dangeroustux\\levels\\level%d.txt", PREFIX, id);
							sprintf(pimage, "%s\\dangeroustux\\images\\", PREFIX);
						#endif
					#else
						char path[1024];
						char pimage[1024];
						#ifndef PREFIX
							sprintf(path, "./levels/level%d.txt", id);
							sprintf(pimage, "./images/");
						#else
							sprintf(path, "%s/share/games/dangeroustux/levels/level%d.txt", PREFIX, id);
							sprintf(pimage, "%s/share/games/dangeroustux/images/", PREFIX);
						#endif
					#endif

					string nimage; // nome da imagem
					ifstream file(path, ifstream::binary);
					if (!file)
						throw "CLevel: Não foi possível ler o arquivo de level\n";
					getline(file, nimage);
					sprintf(path, "%s%s", pimage, nimage.c_str());
					
					bg_path = path;
					bg = new CBackground(optimize_surface_alpha(IMG_Load(path)));
					if (!bg->surface)
					{
						sprintf(path, "CLevel: não foi possível abrir background %s%s\n", pimage, nimage.c_str());
						throw path;
					}
					
					vector <int> tileset;
					string line;
					int i;
					
					for (i = 0; i < 10; i++)
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
					
					if (i < 10)
						throw "CLevel: falha ao ler linhas do mapa, lido menos de 10 linhas\n";
					
					if (!map->read(tileset))
						throw "CLevel: não foi possível iniciar o mapa\n";

					cam->set_limit(map->get_dimension());
					player->set_map(map);
					
					// verifica se já não foi carregado a imagem dos tiles
					if (!map->surface)
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
						map->surface = optimize_surface_alpha(IMG_Load(path));
						if (!map->surface)
							throw SDL_GetError();
					}

					// procura o caractere P no mapa e reinicia o player
					int tile;
					for (i = 0; (tile = map->get_tile(i)) >= 0; i++)
						if (tile == 'P')
						{
							// se é o tile de respawn do jogador, então defina o respawn
							SVect p;
							p.x = (i % map->get_width()) * map->get_tilesize();
							p.y = (i / map->get_width()) * map->get_tilesize();
							player->set_respawn(p);
							player->reset();
							player->gun.shot.clear_targets();
							player->gun.set_gun(false);
							player->jetpack.set_jetpack(false);
							break;
						}
					
					if (i >= map->get_width() * map->get_height())
						throw "CLevel: player NÃO está no mapa, cadê o caractere P?\n";
					
					for (i = 0; (tile = map->get_tile(i)) >= 0; i++)
					{
						if (tile == 'G')
						{
							SVect ip;
							ip.x = (i % map->get_width()) * map->get_tilesize();
							ip.y = (i / map->get_width()) * map->get_tilesize();
							CGameEntity * walker = new CWalkerAlien(player, 8 * map->get_tilesize(), ip);
							aliens.push_back(walker);
							player->gun.shot.add_target(walker);
						}
						else if (tile == 'F')
						{
							SVect ip;
							ip.x = (i % map->get_width()) * map->get_tilesize();
							ip.y = (i / map->get_width()) * map->get_tilesize();
							CGameEntity * flyer = new CFlyerAlien(player, 2.5 * map->get_tilesize(), ip);
							aliens.push_back(flyer);
							player->gun.shot.add_target(flyer);
						}
						else if (tile == 'K')
						{
							SVect p;
							p.x = (i % map->get_width()) * map->get_tilesize();
							p.y = (i / map->get_width()) * map->get_tilesize();
							kernel_signal.set_kernel_pos(p);
							kernel_signal.show(false);
							kernel_signal.set_cam(cam);
						}
						else if (tile == 'E')
						{
							SVect p;
							p.x = (i % map->get_width()) * map->get_tilesize();
							p.y = (i / map->get_width()) * map->get_tilesize();
							exit_signal.set_exit_pos(p);
							exit_signal.show(false);
							exit_signal.set_cam(cam);
						}
					}
					
					int ts = map->get_tilesize();
					map->remove_tile('.'); // remove o tile vazio da visão
					map->remove_tile('P'); // remove o jogador da visão
					map->remove_tile('F'); // remove o walker alien da visão
					map->remove_tile('G'); // remove o flyer alien da visão
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
					a.add_frame((SDL_Rect){0,ts * 2,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts,ts * 2,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts * 2,ts * 2,ts,ts}, 4);
					map->add_animation(a, 'o');

					a.clear_frames();
					a.add_frame((SDL_Rect){ts * 3,ts * 2,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts * 4,ts * 2,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts * 5,ts * 2,ts,ts}, 4);
					map->add_animation(a, 'p');
					
					a.clear_frames();
					a.add_frame((SDL_Rect){ts*8,ts,ts,ts}, 4); // início
					a.add_frame((SDL_Rect){ts*9,ts,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*6,ts,ts,ts}, 4); // centro
					a.add_frame((SDL_Rect){ts*10,ts,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*11,ts,ts,ts}, 4); // final
					a.add_frame((SDL_Rect){ts*10,ts,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*6,ts,ts,ts}, 4); // centro
					a.add_frame((SDL_Rect){ts*9,ts,ts,ts}, 4);
					map->add_animation(a, 'q');

					a.clear_frames();
					a.add_frame((SDL_Rect){ts*8,ts*2,ts,ts}, 4); // início
					a.add_frame((SDL_Rect){ts*9,ts*2,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*7,ts,ts,ts}, 4); // centro
					a.add_frame((SDL_Rect){ts*10,ts*2,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*11,ts*2,ts,ts}, 4); // final
					a.add_frame((SDL_Rect){ts*10,ts*2,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*7,ts,ts,ts}, 4); // centro
					a.add_frame((SDL_Rect){ts*9,ts*2,ts,ts}, 4);
					map->add_animation(a, 'r');
					
					a.clear_frames();
					a.add_frame((SDL_Rect){0,ts*3,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts,ts*3,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*2,ts*3,ts,ts}, 4);
					a.add_frame((SDL_Rect){ts*3,ts*3,ts,ts}, 4);
					map->add_animation(a, 'K');

					#if _WIN32 || _WIN64 || __MINGW32__
						#ifndef PREFIX
							sprintf(path, "%s\\fonts\\inhouseedition.ttf", p2);
						#else
							sprintf(path, "%s\\dangeroustux\\fonts\\inhouseedition.ttf", PREFIX);
						#endif
					#else
						#ifndef PREFIX
							sprintf(path, "./fonts/inhouseedition.ttf");
						#else
							sprintf(path, "%s/share/games/dangeroustux/fonts/inhouseedition.ttf", PREFIX);
						#endif
					#endif
					// define a fonte para CWriter
					if (CWriter::instance()->set_font(path, 55) == 0)
						throw "CLevel: Não conseguiu abrir fonte\n";
					
					// verifica se já foi adicionado algum widget
					if (widget && widget->child_size() > 0) // se sim, pula adicionar de novo
					{
						CLabelNumber * num_level = static_cast<CLabelNumber *>(widget->get_child("num_level"));
						num_level->set_value(id);
						widget->show();
						static_cast<CLabel *>(widget->get_child("jetpack"))->show(false);
						static_cast<CLabel *>(widget->get_child("jetpack_bar"))->show(false);
						static_cast<CLabel *>(widget->get_child("gun"))->show(false);
						static_cast<CLabel *>(widget->get_child("gun_img"))->show(false);
						static_cast<CLabel *>(widget->get_child("go_exit"))->show(false);
						set_state(1); // vai para executando a fase
						break;
					}
					else if (!widget)
						throw "CLevel: Erro no widget\n";
					
					
					CLabel * go_exit = new CLabel("GO THROUGH THE EXIT!", (SDL_Color){255,255,0,0});
					widget->add_child(go_exit);
					go_exit->set_id("go_exit");
					go_exit->set_pos(SVect(318, 12 * map->get_tilesize()));
					
					CLabel * score = new CLabel("SCORE: ", (SDL_Color){0,255,0,0});
					widget->add_child(score);
					score->set_id("score");
					score->set_pos(SVect(125,0));
	
					CLabel * lives = new CLabel("LIVES: ", (SDL_Color){0,255,0,0});
					widget->add_child(lives);
					lives->set_id("lives");
					lives->set_pos(SVect(320 + score->get_pos().x,0));

					CLabel * level = new CLabel("LEVEL: ", (SDL_Color){0,255,0,0});
					widget->add_child(level);
					level->set_id("level");
					level->set_pos(SVect(270 + lives->get_pos().x,0));
	
					CLabel * jetpack = new CLabel("JETPACK: ", (SDL_Color){0,255,0,0});
					widget->add_child(jetpack);
					jetpack->set_id("jetpack");
					jetpack->set_pos(SVect(105, 11 * map->get_tilesize())); 
	
					CLabel * gun = new CLabel("GUN ", (SDL_Color){0,255,0,0});
					widget->add_child(gun);
					gun->set_id("gun");
					gun->set_pos(SVect(720, 11 * map->get_tilesize()));

					CLabel * gun_img = new CLabel(" ", (SDL_Color){0,0,0,0});
					widget->add_child(gun_img);
					gun_img->set_pos(SVect(gun->get_pos().x + gun->get_surface()->w, 11 * map->get_tilesize()));
					gun_img->set_id("gun_img");
					#if _WIN32 || _WIN64 || __MINGW32__
						#ifndef PREFIX
							sprintf(path, "%s\\images\\gun.png", p2);
						#else
							sprintf(path, "%s\\dangeroustux\\images\\gun.png", PREFIX);
						#endif
					#else
						#ifndef PREFIX
							sprintf(path, "./images/gun.png");
						#else
							sprintf(path, "%s/share/games/dangeroustux/images/gun.png", PREFIX);
						#endif
					#endif
					gun_img->set_surface(optimize_surface_alpha(IMG_Load(path)));
					if (!gun_img->get_surface())
						throw SDL_GetError();

					CBar * bar = new CBar(100.0, 384,25);
					widget->add_child(bar);
					bar->color_bg = SDL_MapRGB(screen->format, 255, 255, 255);
					bar->color_bar = SDL_MapRGB(screen->format, 255, 0, 0);
					bar->set_pos(SVect(jetpack->get_pos().x + jetpack->get_surface()->w,540));
					bar->set_id("jetpack_bar");

					CLabelNumber * num_score = new CLabelNumber(0, (SDL_Color){0,255,0,0}, 6);
					widget->add_child(num_score);
					num_score->set_id("num_score");
					num_score->set_pos(SVect(score->get_pos().x + score->get_surface()->w, 0));

					CLabelNumber * num_lives = new CLabelNumber(player->get_lives(), (SDL_Color){0,255,0,0}, 2);
					widget->add_child(num_lives);
					num_lives->set_id("num_lives");
					num_lives->set_pos(SVect(lives->get_pos().x + lives->get_surface()->w, 0));
	
					CLabelNumber * num_level = new CLabelNumber(id, (SDL_Color){0,255,0,0}, 2);
					widget->add_child(num_level);
					num_level->set_id("num_level");
					num_level->set_pos(SVect(level->get_pos().x + level->get_surface()->w, 0));

					widget->set_id("main_window");
					widget->show();
					gun->show(false);
					jetpack->show(false);
					bar->show(false);
					gun_img->show(false);
					go_exit->show(false);
					set_state(RUNNING); // vai para executando a fase
					break;
				}

				case RUNNING: // executando a fase
					cam->lookat(player->get_pos());
					
					// se passou de fase
					if (player->get_state() == FINISHED_LEVEL)
					{
						widget->show(false);
						set_state(NEXT_LEVEL);
						break;
					}
					
					if (player->jetpack.has_jetpack())
					{
						CBar * jetpack_bar = static_cast<CBar *>(widget->get_child("jetpack_bar"));
						CLabel * jetpack = static_cast<CLabel *>(widget->get_child("jetpack"));
						jetpack_bar->set_size(player->jetpack.get_fuel());

						jetpack_bar->show();
						jetpack->show();
						
						if (player->jetpack.has_fuel() == false)
						{
							jetpack_bar->show(false);
							jetpack->show(false);
						}
					}
					
					if (player->gun.has_gun())
					{
						CLabel * gun = static_cast<CLabel *>(widget->get_child("gun"));
						CLabel * gun_img = static_cast<CLabel *>(widget->get_child("gun_img"));
						
						gun->show();
						gun_img->show();
					}

					if (player->has_kernel())
						widget->get_child("go_exit")->show();
					
					static_cast<CLabelNumber *>(widget->get_child("num_score"))->set_value(player->score.get_score());
					static_cast<CLabelNumber *>(widget->get_child("num_lives"))->set_value(player->get_lives());
					
					if (player->get_state() != WAITING)
					{
						SDL_Rect a, b;
						SVect p1, p2;
						
						widget->update();
						map->update_animation();
						player->gun.update();
						for (vector <CGameEntity *>::iterator i = aliens.begin(); i != aliens.end(); i++)
						{
							if ((*i))
								(*i)->update();
						}
						
						player->update();
						
						kernel_signal.update();
						exit_signal.update();
						
						if (player->has_kernel())
							kernel_signal.show(false);
						
						CSoundPlayer::instance()->update();
						CGameEventManager::instance()->update();
						
						for (vector <CGameEntity *>::iterator i = aliens.begin(); i != aliens.end(); i++)
						{
							p1 = player->get_pos();
							p2 = (*i)->get_pos();
							
							a = player->get_dim();
							a.x += p1.x;
							a.y += p1.y;
							
							b = (*i)->get_dim();
							b.x += p2.x;
							b.y += p2.y;
							
							if (((*i)->get_state() != DYING_ALIEN && (*i)->get_state() != INACTIVE_ALIEN) && 
							(player->get_state() != DYING_PLAYER && player->get_state() != FINISHED_LEVEL) &&
							boudingbox(a, b))
							{
								SGameEvent ev;
								ev.receiver = player;
								ev.event = "touched_alien";
								player->gun.shot.rem_target((*i));
								CGameEventManager::instance()->add_event(ev);

								ev.receiver = (*i);
								ev.event = "touched_player";
								CGameEventManager::instance()->add_event(ev);
							}
						}
					}
					else
						player->update();

					break;

				case NEXT_LEVEL: // esperando nova fase
				break;
			}
			
			return get_state();
		}
};


CPlayer * CLevel::player = 0;
SDL_Surface * CLevel::screen = 0;
CWidget * CLevel::widget = 0;
CCamera * CLevel::cam = 0;

#endif

