#include "level.hpp"

CPlayer * CLevel::player = 0;
Widget * CLevel::widget = 0;
Camera * CLevel::cam = 0;
SDL_Renderer * CLevel::renderer = 0;



CLevel::CLevel ( SDL_Renderer * r, int tilesize, int i ): kernel_signal(r), exit_signal(r)
{
	renderer = r;
	widget_color = (SDL_Color){0,0,0,255};

	if (!widget)
		widget = new Widget("level_window");

	id = i;
	map = new TileMapView(tilesize);
	bg = 0;
}

CLevel::~CLevel (  )
{
	unload();
	delete_widget();
}

void CLevel::delete_widget (  )
{
	if (widget)
	{
		Widget * w = widget->get_child(0);
		for (int i(0); w; i++, w = widget->get_child(i))
			delete w;
	
		delete widget;
		widget = 0;
	}
}
	
void CLevel::unload (  )
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

std::string CLevel::get_bg_path (  )
{
	return bg_path;
}

std::string CLevel::get_level_path (  )
{
	return level_path;
}

TileMapView * CLevel::get_map (  )
{
	return map;
}

int CLevel::get_id (  )
{
	return id;
}

void CLevel::draw (  )
{
	bg->draw_hor(renderer, cam);


	map->draw(renderer, cam);
	player->gun.draw(cam, renderer);
	player->draw(cam, renderer);
	kernel_signal.draw(renderer);
	exit_signal.draw(renderer);

	for (vector <CGameEntity *>::iterator i = aliens.begin(); i != aliens.end(); i++)
		(*i)->draw(cam, renderer);

	if (widget)
		widget->draw(renderer);
}

void CLevel::input ( SDL_Event & event )
{
	widget->input(event);
	player->input(event);
	for (int i(0); i < aliens.size(); i++)
		aliens[i]->input(event);
}

int CLevel::update (  )
{
	switch (get_state())
	{
		case INIT_LEVEL: // inicializando
		{
			SDL_Surface * aux = 0;
			
			#if _WIN32 || _WIN64
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
			
			level_path = path;
			std::string nimage; // nome da imagem
			ifstream file(path);
			if (!file)
				throw "CLevel: Não foi possível ler o arquivo de level\n";
			getline(file, nimage);
			sprintf(path, "%s%s", pimage, nimage.c_str());
			
			bg_path = path;

			bg = new Background(IMG_LoadTexture(renderer, path));
			if (!bg->get_texture())
			{
				sprintf(path, "CLevel: não foi possível abrir background %s%s\n", pimage, nimage.c_str());
				throw path;
			}

			vector <int> tileset;
			std::string line;
			int i, size = 0;
			
			for (i = 0; !file.eof(); i++)
			{
				line = "";
				getline(file, line);
				int j;
				
				if (size == 0)
					size = line.size();
				else	if (line.size() != size || file.eof())
					break;
				
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
			file.close();
			
			if (i < 13)
				throw "CLevel: falha ao ler linhas do mapa, lido menos de 13 linhas\n";
			
			if (!map->read(tileset))
				throw "CLevel: não foi possível iniciar o mapa\n";

			cam->set_limit(map->get_dimension());
			player->set_map(map);
			
			// verifica se já não foi carregado a imagem dos tiles
				if (!map->texture)
			{
				#if _WIN32 || _WIN64
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

				map->texture = IMG_LoadTexture(renderer, path);
				if (!map->texture)
					throw SDL_GetError();
			}

			// procura o caractere P no mapa e reinicia o player
			int tile;
			for (i = 0; (tile = map->get_tile(i)) >= 0; i++)
				if (tile == 'P')
				{
					// se é o tile de respawn do jogador, então defina o respawn
					Vect p;
					p.x = (i % map->get_width()) * map->get_tilesize();
					p.y = (i / map->get_width()) * map->get_tilesize();
					player->set_respawn(p);
					player->reset();
					player->gun.shot.clear_targets();
					player->gun.set_gun(false);
					player->jetpack.set_jetpack(false);
					break;
				}
			
			#ifndef EMSCRIPTEN
			if (i >= map->get_width() * map->get_height())
				throw "CLevel: player NÃO está no mapa, cadê o caractere P?\n";
			#endif
			
			for (i = 0; (tile = map->get_tile(i)) >= 0; i++)
			{
				if (tile == 'G')
				{
					Vect ip;
					ip.x = (i % map->get_width()) * map->get_tilesize();
					ip.y = (i / map->get_width()) * map->get_tilesize();
					CGameEntity * walker = new CWalkerAlien(renderer, player, 8 * map->get_tilesize(), ip);
					aliens.push_back(walker);
					player->gun.shot.add_target(walker);
				}
				else if (tile == 'F')
				{
					Vect ip;
					ip.x = (i % map->get_width()) * map->get_tilesize();
					ip.y = (i / map->get_width()) * map->get_tilesize();
					CGameEntity * flyer = new CFlyerAlien(renderer, player, 2.5 * map->get_tilesize(), ip);
					aliens.push_back(flyer);
					player->gun.shot.add_target(flyer);
				}
				else if (tile >= 'A' && tile <= 'D')
				{
					Vect p;
					p.x = (i % map->get_width()) * map->get_tilesize();
					p.y = (i / map->get_width()) * map->get_tilesize();
					CGameEntity * gyro = new CGyroAlien(renderer, player, p, tile);
					aliens.push_back(gyro);
				}
				else if (tile == 'K')
				{
					Vect p;
					p.x = (i % map->get_width()) * map->get_tilesize();
					p.y = (i / map->get_width()) * map->get_tilesize();
					kernel_signal.set_kernel_pos(p);
					kernel_signal.show(false);
					kernel_signal.set_cam(cam);
				}
				else if (tile == 'E')
				{
					Vect p;
					p.x = (i % map->get_width()) * map->get_tilesize();
					p.y = (i / map->get_width()) * map->get_tilesize();
					exit_signal.set_exit_pos(p);
					exit_signal.show(false);
					exit_signal.set_cam(cam);
				}
				else if (tile == 'q') // se é o jetpack
				{
					Vect p;
					p.x = (i % map->get_width()) * map->get_tilesize();
					p.y = (i / map->get_width()) * map->get_tilesize();
					jetpacks.push_back(p);
				}
			}
			
			printf("CLevel: tem %d aliens\n", int(aliens.size()));
			
			int ts = map->get_tilesize();
			map->remove_tile('.'); // remove o tile vazio da visão
			map->remove_tile('P'); // remove o jogador da visão
			map->remove_tile('F'); // remove o flyer alien da visão
			map->remove_tile('G'); // remove o walker alien da visão
			map->set_source('L', (SDL_Rect){ts*11,ts*3,ts,ts});
			map->set_source('M', (SDL_Rect){0,ts*4,ts,ts});
			map->set_source('N', (SDL_Rect){ts,ts*4,ts,ts});
			map->set_source('O', (SDL_Rect){ts*2,ts*4,ts,ts});
			map->set_source('Q', (SDL_Rect){ts*11,ts*4,ts,ts});
			map->set_source('R', (SDL_Rect){0,ts*5,ts,ts});
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
			map->set_source('v', (SDL_Rect){ts*4,ts*3,ts,ts});
			map->set_source('w', (SDL_Rect){ts*5,ts*3,ts,ts});
			map->set_source('0', (SDL_Rect){ts*6,ts*3,ts,ts}); // tile de neve unico
			map->set_source('1', (SDL_Rect){ts*8,ts*3,ts,ts});
			map->set_source('2', (SDL_Rect){ts*9,ts*3,ts,ts});
			map->set_source('3', (SDL_Rect){ts*10,ts*3,ts,ts});
			map->set_source('4', (SDL_Rect){ts*7,ts*3,ts,ts}); // tile de gramado
		
			CAnimatedTile a;
			a.add_frame(map->texture, (SDL_Rect){0,ts * 2,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts,ts * 2,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts * 2,ts * 2,ts,ts}, 40);
			map->add_animation(a, 'o'); // tile de fogo
			
			a.clear_frames();
			a.add_frame(map->texture, (SDL_Rect){ts * 3,ts * 2,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts * 4,ts * 2,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts * 5,ts * 2,ts,ts}, 40);
			map->add_animation(a, 'p'); // macarrão roxo
			
			a.clear_frames();
			a.add_frame(map->texture, (SDL_Rect){ts*8,ts,ts,ts}, 40); // início
			a.add_frame(map->texture, (SDL_Rect){ts*9,ts,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*6,ts,ts,ts}, 40); // centro
			a.add_frame(map->texture, (SDL_Rect){ts*10,ts,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*11,ts,ts,ts}, 40); // final
			a.add_frame(map->texture, (SDL_Rect){ts*10,ts,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*6,ts,ts,ts}, 40); // centro
			a.add_frame(map->texture, (SDL_Rect){ts*9,ts,ts,ts}, 40);
			map->add_animation(a, 'q'); // jetpack
			
			a.clear_frames();
			a.add_frame(map->texture, (SDL_Rect){ts*8,ts*2,ts,ts}, 40); // início
			a.add_frame(map->texture, (SDL_Rect){ts*9,ts*2,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*7,ts,ts,ts}, 40); // centro
			a.add_frame(map->texture, (SDL_Rect){ts*10,ts*2,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*11,ts*2,ts,ts}, 40); // final
			a.add_frame(map->texture, (SDL_Rect){ts*10,ts*2,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*7,ts,ts,ts}, 40); // centro
			a.add_frame(map->texture, (SDL_Rect){ts*9,ts*2,ts,ts}, 40);
			map->add_animation(a, 'r'); // arma laser
			
			a.clear_frames();
			a.add_frame(map->texture, (SDL_Rect){0,ts*3,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts,ts*3,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*2,ts*3,ts,ts}, 40);
			a.add_frame(map->texture, (SDL_Rect){ts*3,ts*3,ts,ts}, 40);
			map->add_animation(a, 'K'); // kernel
			
			a.clear_frames();
			a.add_frame(map->texture, (SDL_Rect){ts*6,ts*4,ts,ts}, 60);
			a.add_frame(map->texture, (SDL_Rect){ts*5,ts*4,ts,ts}, 60);
			a.add_frame(map->texture, (SDL_Rect){ts*4,ts*4,ts,ts}, 60);
			a.add_frame(map->texture, (SDL_Rect){ts*3,ts*4,ts,ts}, 60);
			map->add_animation(a, 'x'); // água fervente
			
			a.clear_frames();
			a.add_frame(map->texture, (SDL_Rect){ts*10,ts*4,ts,ts}, 60);
			a.add_frame(map->texture, (SDL_Rect){ts*9,ts*4,ts,ts}, 60);
			a.add_frame(map->texture, (SDL_Rect){ts*8,ts*4,ts,ts}, 60);
			a.add_frame(map->texture, (SDL_Rect){ts*7,ts*4,ts,ts}, 60);
			map->add_animation(a, 'y'); // piche
			
			
			
			#if _WIN32 || _WIN64
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
			// define a fonte para Writer
			if (Writer::instance()->load_font(path, path, 55) == 0)
				throw "CLevel: Não conseguiu abrir fonte\n";

			// verifica se já foi adicionado algum widget
			if (widget && widget->child_size() > 0) // se sim, pula adicionar de novo
			{
				GuiLabelNumber * num_level = static_cast<GuiLabelNumber *>(widget->get_child("num_level"));
				num_level->set_value(id);
				widget->show();
				static_cast<GuiLabel *>(widget->get_child("jetpack"))->show(false);
				static_cast<GuiLabel *>(widget->get_child("jetpack_bar"))->show(false);
				static_cast<GuiLabel *>(widget->get_child("gun"))->show(false);
				static_cast<GuiLabel *>(widget->get_child("gun_img"))->show(false);
				//static_cast<GuiLabel *>(widget->get_child("go_exit"))->show(false);
				set_state(1); // vai para executando a fase
				break;
			}
			else if (!widget)
				throw "CLevel: Erro no widget\n";
			
			/*
			GuiLabel * go_exit = new GuiLabel("GO THROUGH THE EXIT!", (SDL_Color){255,255,0,0});
			widget->add_child(go_exit);
			go_exit->set_id("go_exit");
			go_exit->set_pos(Vect(318, 12 * map->get_tilesize()));
			//removido também linhas lá em baixo
			*/
			
			GuiLabel * score = new GuiLabel("SCORE: ", widget_color);
			widget->add_child(score);
			score->set_id("score");
			score->set_pos(Vect(125,0));

			GuiLabel * lives = new GuiLabel("LIVES: ", widget_color);
			widget->add_child(lives);
			lives->set_id("lives");
			lives->set_pos(Vect(320 + score->get_pos().x,0));

			GuiLabel * level = new GuiLabel("LEVEL: ", widget_color);
			widget->add_child(level);
			level->set_id("level");
			level->set_pos(Vect(270 + lives->get_pos().x,0));

			GuiLabel * jetpack = new GuiLabel("JETPACK: ", widget_color);
			widget->add_child(jetpack);
			jetpack->set_id("jetpack");
			jetpack->set_pos(Vect(105, map->get_tilesize())); 

			GuiLabel * gun = new GuiLabel("GUN ", widget_color);
			widget->add_child(gun);
			gun->set_id("gun");
			gun->set_pos(Vect(720, map->get_tilesize()));

			GuiLabel * gun_img = new GuiLabel(" ", widget_color);
			widget->add_child(gun_img);
			gun_img->set_pos(Vect(gun->get_pos().x + gun->get_texture_width(), map->get_tilesize()));
			gun_img->set_id("gun_img");
			#if _WIN32 || _WIN64
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
			
			gun_img->set_texture(IMG_LoadTexture(renderer, path));
			if (!gun_img->get_texture())
				throw SDL_GetError();

			GuiBar * bar = new GuiBar(100.0, 384,25);
			widget->add_child(bar);
			bar->color_bg = 0xFFFFFFFF;
			bar->color_bar = 0xFF000000;
			bar->set_pos(Vect(jetpack->get_pos().x + jetpack->get_texture_width(),map->get_tilesize() + 12));
			bar->set_id("jetpack_bar");

			GuiLabelNumber * num_score = new GuiLabelNumber(0, widget_color, 6);
			widget->add_child(num_score);
			num_score->set_id("num_score");
			num_score->set_pos(Vect(score->get_pos().x + score->get_texture_width(), 0));

			GuiLabelNumber * num_lives = new GuiLabelNumber(player->get_lives(), widget_color, 2);
			widget->add_child(num_lives);
			num_lives->set_id("num_lives");
			num_lives->set_pos(Vect(lives->get_pos().x + lives->get_texture_width(), 0));

			GuiLabelNumber * num_level = new GuiLabelNumber(id, widget_color, 2);
			widget->add_child(num_level);
			num_level->set_id("num_level");
			num_level->set_pos(Vect(level->get_pos().x + level->get_texture_width(), 0));

			widget->show();
			gun->show(false);
			jetpack->show(false);
			bar->show(false);
			gun_img->show(false);
			//go_exit->show(false);
			set_state(RUNNING_LEVEL); // vai para executando a fase
			break;
		}

		case RUNNING_LEVEL: // executando a fase
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
				GuiBar * jetpack_bar = static_cast<GuiBar *>(widget->get_child("jetpack_bar"));
				GuiLabel * jetpack = static_cast<GuiLabel *>(widget->get_child("jetpack"));
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
				GuiLabel * gun = static_cast<GuiLabel *>(widget->get_child("gun"));
				GuiLabel * gun_img = static_cast<GuiLabel *>(widget->get_child("gun_img"));
				
				gun->show();
				gun_img->show();
			}
			
			/*
			if (player->has_kernel())
				widget->get_child("go_exit")->show();
			*/
			static_cast<GuiLabelNumber *>(widget->get_child("num_score"))->set_value(player->score.get_score());
			static_cast<GuiLabelNumber *>(widget->get_child("num_lives"))->set_value(player->get_lives());
			
			if (player->get_state() != WAITING)
			{
				SDL_Rect a, b;
				Vect p1, p2;
				
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
				
				SoundPlayer::instance()->update();
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
					boundingbox(a, b))
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
			{
				for (int i(0); i < jetpacks.size(); i++)
					if (map->get_tile(int(jetpacks[i].x), int(jetpacks[i].y)) == '.')
						map->set_tile(int(jetpacks[i].x), int(jetpacks[i].y), 'q');
				
				
				GuiBar * jetpack_bar = static_cast<GuiBar *>(widget->get_child("jetpack_bar"));
				GuiLabel * jetpack = static_cast<GuiLabel *>(widget->get_child("jetpack"));

				jetpack_bar->show(false);
				jetpack->show(false);
				
				player->jetpack.set_jetpack(false);
				player->update();
			}

			break;

		case NEXT_LEVEL: // esperando nova fase
		break;
	}
	
	return get_state();
}



