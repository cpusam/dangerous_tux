#include "gamescreen.hpp"

#ifndef USE_SDL2
	CGameScreen::CGameScreen ( SDL_Surface * s, Camera * c, int ts )
#else
	CGameScreen::CGameScreen ( SDL_Window * w, SDL_Renderer * r, Camera * c, int ts ): gameover(r), highscore(r), title(r), credits(r), transition(r), introduction(r)
#endif
{
	curr_level = -1;
	tilesize = ts;
	#ifndef USE_SDL2
		screen = s;
	#else
		window = w;
		renderer = r;
	#endif
	cam = c;

	#ifndef USE_SDL2
		player = new CPlayer();
	#else
		player = new CPlayer(renderer);
	#endif
	
	#ifndef USE_SDL2
		if (!screen)
			throw "CGameScreen: screen é nulo\n";
	#else
		if (!w || !r)
			throw "CGameScreen: window ou renderer nulo\n";
	#endif
	
	if (!player)
		throw "CGameScreen: player é nulo\n";
		
	if (!cam)
		throw "CGameScreen: cam é nulo\n";

	// pre seta a transição
	#ifndef USE_SDL2
		transition.set_cam(cam, screen);
	#else
		transition.set_cam(cam, renderer);
	#endif
	transition.set_player(player);
	
	#if _WIN32 || _WIN64
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

	if (Writer::instance()->load_font(path, path, 80) == 0)
		throw "CGameScreen: não foi possí­vel carregar fonte\n";
	
	#if USE_SDL2
		Writer::instance()->set_renderer(r);
	#endif
	
	#ifndef USE_SDL2
		pause = new GuiLabel("PAUSE!", (SDL_Color){0,0,0,0});
		pause->set_pos(Vect((screen->w - pause->get_surface()->w)/2, (screen->h - pause->get_surface()->h)/2));
		name_msg = new GuiLabel("YOU GOT A HIGH SCORE!\n Enter your name:", (SDL_Color){255,255,0, 0});
		name_msg->set_pos(Vect(230, 110));
		textinput = new GuiTextInput(80, (SDL_Color){255,0,0,0});
		textinput->set_pos(Vect(name_msg->get_pos().x, name_msg->get_pos().y + name_msg->get_surface()->h));
		final_msg = new GuiLabel("   final_msg\nPRESS ANY KEY", (SDL_Color){255,0,0,0});
		final_msg->set_pos(Vect((screen->w - final_msg->get_surface()->w)/2, (screen->h - final_msg->get_surface()->h)/2));
	#else
		int width, height;
		SDL_RenderGetLogicalSize(renderer, &width, &height);
		
		pause = new GuiLabel("PAUSE!", (SDL_Color){0,0,0,0});
		pause->set_pos(Vect((width - pause->get_texture_width())/2, (height - pause->get_texture_height())/2));
		name_msg = new GuiLabel("YOU GOT A HIGH SCORE!\n Enter your name:", (SDL_Color){255,255,0, 0});
		name_msg->set_pos(Vect(230, 110));
		textinput = new GuiTextInput(80, (SDL_Color){255,0,0,0});
		textinput->set_pos(Vect(name_msg->get_pos().x, name_msg->get_pos().y + name_msg->get_texture_height()));
		final_msg = new GuiLabel("   FINAL\npress any key", (SDL_Color){255,0,0,0});
		final_msg->set_pos(Vect((width - final_msg->get_texture_width())/2, (height - final_msg->get_texture_height())/2));
	#endif
	
	widget.add_child(pause);
	widget.add_child(name_msg);
	widget.add_child(textinput);
	widget.add_child(final_msg);
	widget.show();
	widget.show_child(false);
	
	save[0] = new CSaveGame("DT_save1");
	save[1] = new CSaveGame("DT_save2");
	save[2] = new CSaveGame("DT_save3");
	
	#ifndef USE_SDL2
		menu = new CGameMenu(save);
	#else
		menu = new CGameMenu(save);
	#endif
	
	any_key = enter_key = pause_key = 0;
	
	#if _WIN32 || _WIN64
		#ifndef PREFIX
			sprintf(path, "%s\\images\\chora_logo.png", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\images\\chora_logo.png", PREFIX);
		#endif
	#else
		#ifndef PREFIX
			sprintf(path, "./images/chora_logo.png");
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/chora_logo.png", PREFIX);
		#endif
	#endif
	
	#ifndef USE_SDL2
		SDL_Surface * aux = IMG_Load(path);
		chora.add_frame(aux, (SDL_Rect){0,0,aux->w,aux->h}, 60);
	#else
		SDL_Texture * aux = IMG_LoadTexture(r, path);
		chora.add_frame(aux, (SDL_Rect){0,0,texture_width(aux),texture_height(aux)}, 60);
	#endif
	chora.set_repeat(false);
	set_state(CHORA_SCREEN);
	//credits.reset();
	//set_state(CREDITS_SCREEN); // tela de créditos
	//set_state(LOAD_GAME);
}

CGameScreen::~CGameScreen (  )
{
	if (levels.size() > 0)
	{
		levels[0]->delete_widget();
		clear_levels();
	}
	
	#ifndef USE_SDL2
		chora.destroy_surfaces();
	#else
		chora.destroy_textures();
	#endif
	delete save[0];
	delete save[1];
	delete save[2];
	delete name_msg;
	delete textinput;
	delete final_msg;
	delete player;
}

void CGameScreen::clear_levels (  )
{
	for (int i(0); i < levels.size(); i++)
	{
		if (levels[i])
			delete levels[i];
	}

	levels.clear();
}

void CGameScreen::input ( SDL_Event & event )
{
	widget.input(event);
	highscore.input(event);

	if (get_state() == MAIN_LOOP || get_state() == PAUSE_SCREEN)
	{
		levels[curr_level]->input(event);
	}
	else if (get_state() == GAMEMENU_SCREEN)
	{
		menu->input(event);
	}
	else if (get_state() == TITLE_SCREEN)
	{
		title.input(event);
	}
	
	if (event.type == SDL_KEYDOWN)
	{
		#ifndef USE_SDL2
			textinput->set_pos(Vect(name_msg->get_pos().x + (name_msg->get_surface()->w - textinput->get_surface()->w)/2, textinput->get_pos().y));
		#else
			textinput->set_pos(Vect(name_msg->get_pos().x + (name_msg->get_texture_width() - textinput->get_texture_width())/2, textinput->get_pos().y));
		#endif
		if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
			enter_key = 1;
			
		if (event.key.keysym.sym == SDLK_p)
		{
			if (pause_key == 0)
			{
				if (get_state() == MAIN_LOOP)
				{
					pause_key = 1;
					pause->show(true);
					set_state(PAUSE_SCREEN);
				}
			}
			else
			{
				if (get_state() == PAUSE_SCREEN)
				{
					pause_key = 0;
					pause->show(false);
					set_state(MAIN_LOOP);
				}
			}
		}
		else
		{
			any_key = 1;
		}
	}
	
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
			enter_key = 0;
		
		if (event.key.keysym.sym != SDLK_p)
			any_key = 0;
	}
}

void CGameScreen::draw (  )
{
	switch (get_state())
	{
		case CHORA_SCREEN:
			#ifndef USE_SDL2
				SDL_FillRect(screen, NULL, SDL_MapRGBA(screen->format, 0,0,0,255));
				chora.draw(chora_pos.x, chora_pos.y, screen);
			#else
				SDL_SetRenderDrawColor(renderer, 0,0,0,0xff);
				SDL_RenderClear(renderer);
				chora.draw(renderer, chora_pos.x, chora_pos.y);
			#endif
			break;

		case CREDITS_SCREEN:
			#ifndef USE_SDL2
				credits.draw(screen);
			#else
				credits.draw(renderer);
			#endif
			break;

		case INTRODUCTION:
			#ifndef USE_SDL2
				introduction.draw(screen);
			#else
				introduction.draw(renderer);
			#endif
			break;

		case PAUSE_SCREEN:
		case MAIN_LOOP:
			if (curr_level < 0)
				break;
			
			#ifndef USE_SDL2
				SDL_FillRect(screen, NULL, 0x0);
				levels[curr_level]->draw();
				widget.draw(screen);
			#else
				SDL_SetRenderDrawColor(renderer, 0,0,0,0xFF);
				SDL_RenderClear(renderer);
				levels[curr_level]->draw();
				widget.draw(renderer);
			#endif
			break;

		case HIGHSCORE_SCREEN:
			#ifndef USE_SDL2
				SDL_FillRect(screen, NULL, 0x0);
				highscore.draw(screen);
			#else
				SDL_SetRenderDrawColor(renderer, 0,0,0,0xFF);
				SDL_RenderClear(renderer);
				highscore.draw(renderer);
			#endif
			break;

		case TITLE_SCREEN:
			#ifndef USE_SDL2
				title.draw(screen);
			#else
				title.draw(renderer);
			#endif
			break;
		
		case GAMEMENU_SCREEN:
			#ifndef USE_SDL2
				title.draw(screen);
				menu->draw(screen);
			#else
				title.draw(renderer);
				menu->draw(renderer);
			#endif
			break;

		case TRANSITION:
			#ifndef USE_SDL2
				transition.draw(screen);
			#else
				transition.draw();
			#endif
			break;

		case GAMEOVER_SCREEN:
			#ifndef USE_SDL2
				gameover.draw(cam, screen);
			#else
				gameover.draw(cam, renderer);
			#endif
			break;

		default:
			#ifndef USE_SDL2
				SDL_FillRect(screen, NULL, 0x0);
				widget.draw(screen);
			#else
				SDL_SetRenderDrawColor(renderer, 0,0,0,0xFF);
				SDL_RenderClear(renderer);
				widget.draw(renderer);
			#endif
			break;
	}
}

int CGameScreen::update (  )
{
	switch (get_state())
	{
		case CHORA_SCREEN:
			if (any_key || chora.update() == Animation::STOPPED)
			{
				any_key = 0;
				enter_key = 0;
				title.reset();
				set_state(TITLE_SCREEN);
			}
			break;
		
		case CREDITS_SCREEN:
			if (any_key || credits.update() == INACTIVE_CREDITS)
			{
				any_key = 0;
				enter_key = 0;
				menu->reset();
				set_state(GAMEMENU_SCREEN);
				//widget.show_child(false);
				//title.reset();
				//set_state(GAMEMENU_SCREEN);
			}
			break;

		case TITLE_SCREEN:
			if (title.update() == BACKGROUND_GAMETITLE)
			{
				menu->reset();
				set_state(GAMEMENU_SCREEN);
				break;
			}
			break;
		
		case GAMEMENU_SCREEN:
			title.update();
			menu->update();
			if (menu->get_state() == END_GAMEMENU)
			{
				any_key = 0;
				enter_key = 0;
				vector <COption *> option = menu->options.get_option();
				for (int i(0); i < option.size(); i++)
					if (option[i] == menu->options.get_chose_option())
					{
						switch (i)
						{
							case 0 ... 2:
								set_state(LOAD_GAME);
								break;
							
							case 3: // creditos
								credits.reset();
								set_state(CREDITS_SCREEN);
								break;
							
							case 4: // sair do jogo
								set_state(EXIT_SCREEN);
								break;
							
							default:
								break;
						}
						
						break;
					}
			}
			break;

		case LOAD_GAME:
			if (menu->options.get_curr_save()->load()) // se conseguir carregar
			{
				SSaveData d = menu->options.get_curr_save()->get_data();
				int aux[3];
				memset(aux, 0x0, sizeof(aux));
				sscanf(d.curr_level, "%d", &curr_level);
				sscanf(d.lives, "%d", &aux[0]);
				sscanf(d.score, "%d", &aux[1]);
				sscanf(d.score_aux, "%d", &aux[2]);
				player->set_lives(aux[0]);
				player->score.set_score(aux[1]);
				player->score.set_score_aux(aux[2]);
				cout << "CGameScreen: save game carregado do arquivo\n";
			}
			else
			{
				curr_level = -1; // fase '0' será a primeira
				player->set_lives(10); // seta as vidas do jogador
				introduction.reset();
				set_state(INTRODUCTION);
				break;
			}

			set_state(LOAD_LEVELS_SCREEN); // vai para inicializando o level
			break;
		
		case INTRODUCTION: // tela de introdução do jogo
			if (enter_key || introduction.get_state() == INACTIVE_INTRODUCTION)
			{
				any_key = 0;
				enter_key = 0;
				widget.show_child(false);
				set_state(LOAD_LEVELS_SCREEN);
				break;
			}
			
			introduction.update();
			break;
		
		case SAVE_GAME:
			if (player->get_state() != GAMEOVER)
			{
				// não salva na Ãºltima fase
				if (curr_level + 1 < levels.size())
				{
					SSaveData d;
					sprintf(d.curr_level, "%d", curr_level + 1);
					sprintf(d.lives, "%d", player->get_lives());
					sprintf(d.score, "%d", player->score.get_score());
					sprintf(d.score_aux, "%d", player->score.get_score_aux());
					menu->options.get_curr_save()->save(d);
					cout << "CGameScreen salvando jogo\n";

					transition.set_bg(levels[curr_level]->get_bg_path());
					transition.reset(curr_level, levels.size());
					set_state(TRANSITION);
				}
				else
				{
					levels[0]->widget->show(false);
					final_msg->show(true);
					any_key = 0;
					set_state(FINAL_SCREEN); // vai para tela de final_msg
					break;
				}
			}
			else
			{
				any_key = 0;
				gameover.reset();
				set_state(GAMEOVER_SCREEN);
			}
			break;

		case LOAD_LEVELS_SCREEN:
			// limpa os levels antigos se existir
			clear_levels();
			
			#if _WIN32 || _WIN64
				char path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
			#else
				char path[1024]; // o bom e velho char []
			#endif
			for (int i(0); i < 100; i++) // para no máximo 100 levels
			{
				#if _WIN32 || _WIN64
					#ifndef PREFIX
						sprintf(path, "%s\\levels\\level%d.txt", p2, i + 1);
					#else
						sprintf(path, "%s\\dangeroustux\\levels\\level%d.txt", PREFIX, i + 1);
					#endif
				#else
					#ifndef PREFIX
						sprintf(path, "./levels/level%d.txt", i + 1);
					#else
						sprintf(path, "%s/share/games/dangeroustux/levels/level%d.txt", PREFIX, i + 1);
					#endif
				#endif

				ifstream file(path, ifstream::in | ifstream::binary);
				if (file)
				{
					#ifndef USE_SDL2
						CLevel * l = new CLevel(tilesize, i + 1);
					#else
						CLevel * l = new CLevel(renderer, tilesize, i + 1);
					#endif
					
					#ifndef USE_SDL2
						l->screen = screen;
					#endif
					l->player = player;
					l->cam = cam;
					
					levels.push_back(l);
					file.close();
				}
				else
					break;
			}

			cout << "Lido " << levels.size() << " levels" << endl;
			if (levels.size() < 10)
				cout << "CGameScreen: LEMBRAR DE CARREGAR PELO MENOS 10 LEVELS!!!\n";
			if (levels.size() <= 0)
				throw "CGameScreen: sem levels na pasta levels\n";

			if (curr_level == -1)
				set_state(NEXT_LEVEL_SCREEN);
			else
				set_state(INIT_LEVEL_SCREEN);
			break;

		case NEXT_LEVEL_SCREEN:
			curr_level++;
			set_state(INIT_LEVEL_SCREEN);
			break;

		case INIT_LEVEL_SCREEN: // inicializando o level	
			levels[curr_level]->set_state(0); // seta o level para estado de iniciando
			levels[curr_level]->update(); // executa um update para inicializar variáveis

			player->jetpack.set_jetpack(false);
			player->gun.set_gun(false);
			player->set_kernel(false);
			
			widget.show_child(false);
			set_state(MAIN_LOOP);
			break;
		
		case MAIN_LOOP: // main loop
			// verifica se passou da fase
			if (levels[curr_level]->get_state() == NEXT_LEVEL)
			{
				widget.show_child(false);
				any_key = 0;
				levels[curr_level]->unload();
				set_state(SAVE_GAME); // vai para tela de save game
				break;
			}
			
			if (player->get_state() == GAMEOVER) // se deu game over
			{
				levels[curr_level]->widget->show(false);
				widget.show_child(false);
				any_key = 0;
				set_state(SAVE_GAME); // vai para tela save game
				break;
			}

			levels[curr_level]->update();
			break;
		
		case PAUSE_SCREEN:
			break;
		
		case TRANSITION: // transição de fase
			if (transition.update() == 0)
			{
				widget.show_child(false);
				set_state(NEXT_LEVEL_SCREEN);
				break;
			}
			break;
		
		case FINAL_SCREEN: // tela do final_msg do jogo
			if (any_key)
			{
				widget.show_child(false);
				if (highscore.is_highscore(player->score.get_score()))
				{
					enter_key = 0;
					name_msg->show();
					textinput->show();
					set_state(GET_SCORE_NAME); // vai para tela de digitar nome
					#if _WIN32 || _WIN64
						char path[FILENAME_MAX];
						#ifndef PREFIX
							char p2[FILENAME_MAX];
							sprintf(path, "%s\\fonts\\inhouseedition.ttf", _getcwd(p2, sizeof(p2)));
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
					// É preciso redefinir a fonte
					if (Writer::instance()->load_font(path, path, 80) == 0)
						throw "CGameScreen: não conseguiu abrir fonte\n";
					
					#if USE_SDL2
						Writer::instance()->set_renderer(renderer);
					#endif
					break;
				}
				highscore.show();
				any_key = 0;
				set_state(HIGHSCORE_SCREEN); // vai para tela de highscore
			}
			break;
		
		case GAMEOVER_SCREEN: // tela de game over
			if (enter_key)
			{
				widget.show_child(false);
				if (highscore.is_highscore(player->score.get_score()))
				{
					any_key = 0;
					enter_key = 0;
					name_msg->show();
					textinput->show();
					set_state(GET_SCORE_NAME); // vai para tela de digitar nome
					#if _WIN32 || _WIN64
						char path[FILENAME_MAX];
						#ifndef PREFIX
							char p2[FILENAME_MAX];
							sprintf(path, "%s\\fonts\\inhouseedition.ttf", _getcwd(p2, sizeof(p2)));
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
					// É preciso redefinir a fonte
					if (Writer::instance()->load_font(path, path, 80) == 0)
						throw "CGameScreen: não conseguiu abrir fonte\n";
					
					#if USE_SDL2
						Writer::instance()->set_renderer(renderer);
					#endif
					break;
				}

				highscore.show();
				any_key = 0;
				enter_key = 0;
				set_state(HIGHSCORE_SCREEN); // vai para tela de high score
			}
			
			gameover.update();
			break;
		
		case HIGHSCORE_SCREEN: // tela de high score 
			if (any_key)
			{
				widget.show_child(false);
				highscore.show(false);
				player->score.set_score(0);
				any_key = 0;
				enter_key = 0;
				chora.reset();
				set_state(CHORA_SCREEN); // vai para tela de logo
			}
			break;

		case GET_SCORE_NAME: // tela de digitar nome para high score
			if (enter_key)
			{
				SPlayerScore s;
				
				if (textinput->get_str().size() <= 1)
					sprintf(s.name, "PLAYER");
				else
					sprintf(s.name, "%s", textinput->get_str().c_str());
				
				sprintf(s.score, "%d", player->score.get_score());
				sprintf(s.level, "%d", (curr_level == -1)? 1: curr_level + 1);
				highscore.save(s);
				enter_key = 0;
				any_key = 0;
				highscore.show();
				set_state(HIGHSCORE_SCREEN); // vai para high score
			}
			break;
		
		case EXIT_SCREEN:
			break;
		
		default:
			set_state(INTRODUCTION);
			break;
	}
	
	return get_state();
}


