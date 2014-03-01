#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include "gameevent_manager.hpp"
#include "player.hpp"
#include "level.hpp"
#include "highscore.hpp"
#include "savegame.hpp"
#include "gameover.hpp"
#include "gamecredits.hpp"
#include "gameintroduction.hpp"
#include "gametitle.hpp"
#include "gametransition.hpp"
#include "gamemenu.hpp"


// enumeração das telas
enum EScreenState
{
	CREDITS_SCREEN,
	INTRODUCTION,
	TITLE_SCREEN,
	LOAD_GAME,
	SAVE_GAME,
	LOAD_LEVELS_SCREEN,
	NEXT_LEVEL_SCREEN,
	INIT_LEVEL_SCREEN,
	PAUSE_SCREEN,
	MAIN_LOOP,
	TRANSITION,
	FINAL_SCREEN,
	GAMEOVER_SCREEN,
	HIGHSCORE_SCREEN,
	GET_SCORE_NAME
};

class CGameScreen: public CStateMachine
{
	protected:
		int curr_level; // �ndice do level atual
		vector <CLevel *> levels; // os levels
		SDL_Surface * screen;
		CCamera * cam;
		CPlayer * player;
		int any_key, enter_key, pause_key;
		int tilesize;
		
		CGameOver gameover;
		CGameTitle title;
		CGameCredits credits;
		CGameIntroduction introduction;
		CGameTransition transition;
		CSaveGame save;
		CHighScore highscore;
		CWidget window;
		CLabel * pause;
		CLabel * name_msg;
		CLabel * final;
		CTextInput * textinput;

	public:
		CGameScreen ( SDL_Surface * s, CCamera * c, CPlayer * p, int ts )
		{
			curr_level = -1;
			tilesize = ts;
			screen = s;
			cam = c;
			player = p;
			
			if (!screen)
				throw "CGameScreen: screen é nulo\n";
				
			if (!player)
				throw "CGameScreen: player é nulo\n";
				
			if (!cam)
				throw "CGameScreen: cam é nulo\n";

			// pre seta a transição
			transition.set_cam(cam, screen);
			transition.set_player(player);

			// carrega as imagens da tela de título
			title.load();
			
			// carrega as imagens da tela de game over
			gameover.load();
			
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

			if (CWriter::instance()->set_font(path, 80) == 0)
				throw "CGameScreen: não foi possível carregar fonte\n";
			
			pause = new CLabel("PAUSE!", (SDL_Color){0,0,0,0});
			pause->set_pos(SVect((screen->w - pause->get_surface()->w)/2, (screen->h - pause->get_surface()->h)/2));
			name_msg = new CLabel("YOU GOT A HIGH SCORE!\n Enter your name:", (SDL_Color){255,255,0, 0});
			name_msg->set_pos(SVect(230, 110));
			textinput = new CTextInput(80, (SDL_Color){255,0,0,0});
			textinput->set_pos(SVect(name_msg->get_pos().x, name_msg->get_pos().y + name_msg->get_surface()->h));
			final = new CLabel("   FINAL\npress any key", (SDL_Color){255,0,0,0});
			final->set_pos(SVect((screen->w - final->get_surface()->w)/2, (screen->h - final->get_surface()->h)/2));

			window.add_child(pause);
			window.add_child(name_msg);
			window.add_child(textinput);
			window.add_child(final);
			window.show();
			window.show_child(false);
			
			any_key = enter_key = pause_key = 0;
			credits.reset();
			set_state(CREDITS_SCREEN); // tela de créditos
		}
		
		~CGameScreen (  )
		{
			if (levels.size() > 0)
			{
				levels[0]->delete_widget();
				clear_levels();
			}
			
			delete name_msg;
			delete textinput;
			delete final;
		}
		
		void clear_levels (  )
		{
			for (int i(0); i < levels.size(); i++)
			{
				if (levels[i])
					delete levels[i];
			}

			levels.clear();
		}
		
		void input ( SDL_Event & event )
		{
			window.input(event);
			highscore.input(event);

			if (get_state() == MAIN_LOOP || get_state() == PAUSE_SCREEN)
				levels[curr_level]->input(event);
			
			if (event.type == SDL_KEYDOWN)
			{
				textinput->set_pos(SVect(name_msg->get_pos().x + (name_msg->get_surface()->w - textinput->get_surface()->w)/2, textinput->get_pos().y));
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
		
		void draw (  )
		{
			switch (get_state())
			{
				case CREDITS_SCREEN:
					credits.draw(screen);
					break;

				case INTRODUCTION:
					introduction.draw(screen);
					break;

				case PAUSE_SCREEN:
				case MAIN_LOOP:
					if (curr_level < 0)
						break;

					SDL_FillRect(screen, NULL, 0x0);
					levels[curr_level]->draw();
					window.draw(screen);
					break;

				case HIGHSCORE_SCREEN: // high score
					SDL_FillRect(screen, NULL, 0x0);
					highscore.draw(screen);
					break;

				case TITLE_SCREEN:
					title.draw(cam, screen);
					break;

				case TRANSITION:
					transition.draw(screen);
					break;

				case GAMEOVER_SCREEN:
					gameover.draw(cam, screen);
					break;

				default:
					SDL_FillRect(screen, NULL, 0x0);
					window.draw(screen);
					break;
			}
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case CREDITS_SCREEN:
					if (any_key || credits.update() == INACTIVE_CREDITS)
					{
						any_key = 0;
						enter_key = 0;
						window.show_child(false);
						title.reset();
						set_state(TITLE_SCREEN);
					}
					break;

				case INTRODUCTION: // tela de introdução do jogo
					if (enter_key || introduction.get_state() == INACTIVE_INTRODUCTION)
					{
						any_key = 0;
						enter_key = 0;
						window.show_child(false);
						set_state(LOAD_LEVELS_SCREEN);
						break;
					}
					
					introduction.update();
					break;

				case TITLE_SCREEN:
					if (enter_key)
					{
						any_key = 0;
						enter_key = 0;
						set_state(LOAD_GAME);
					}
					
					title.update();
					break;
				
				case LOAD_GAME:
					if (save.load()) // se conseguir carregar
					{
						SSaveData d = save.get_data();
						int aux[3];
						SDL_memset(aux, 0x0, sizeof(aux));
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

				case SAVE_GAME:
					if (player->get_state() != GAMEOVER)
					{
						// não salva na última fase
						if (curr_level + 1 < levels.size())
						{
							SSaveData d;
							sprintf(d.curr_level, "%d", curr_level + 1);
							sprintf(d.lives, "%d", player->get_lives());
							sprintf(d.score, "%d", player->score.get_score());
							sprintf(d.score_aux, "%d", player->score.get_score_aux());
							save.save(d);
							cout << "CGameScreen salvando jogo\n";

							transition.set_bg(levels[curr_level]->get_bg_path());
							transition.reset(curr_level, levels.size());
							set_state(TRANSITION);
						}
						else
						{
							levels[0]->widget->show(false);
							final->show(true);
							any_key = 0;
							set_state(FINAL_SCREEN); // vai para tela de final
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
					
					#if _WIN32 || _WIN64 || __MINGW32__
						char path[FILENAME_MAX];
						char p2[FILENAME_MAX];
						_getcwd(p2, sizeof(p2));
					#else
						char path[1024]; // o bom e velho char []
					#endif
					for (int i(0); i < 100; i++) // para no máximo 100 levels
					{
						#if _WIN32 || _WIN64 || __MINGW32__
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
							CLevel * l = new CLevel(tilesize, i + 1);
							l->screen = screen;
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
					
					window.show_child(false);
					set_state(MAIN_LOOP);
					break;
				
				case MAIN_LOOP: // main loop
					// verifica se passou da fase
					if (levels[curr_level]->get_state() == NEXT_LEVEL)
					{
						window.show_child(false);
						any_key = 0;
						levels[curr_level]->unload();
						set_state(SAVE_GAME); // vai para tela de save game
						break;
					}
					
					if (player->get_state() == GAMEOVER) // se deu game over
					{
						levels[curr_level]->widget->show(false);
						window.show_child(false);
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
						window.show_child(false);
						set_state(NEXT_LEVEL_SCREEN);
						break;
					}
					break;
				
				case FINAL_SCREEN: // tela do final do jogo
					if (any_key)
					{
						window.show_child(false);
						if (highscore.is_highscore(player->score.get_score()))
						{
							enter_key = 0;
							name_msg->show();
							textinput->show();
							set_state(GET_SCORE_NAME); // vai para tela de digitar nome
							#if _WIN32 || _WIN64 || __MINGW32__
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
							// é preciso redefinir a fonte
							if (CWriter::instance()->set_font(path, 80) == 0)
								throw "CGameScreen: não conseguiu abrir fonte\n";
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
						window.show_child(false);
						if (highscore.is_highscore(player->score.get_score()))
						{
							any_key = 0;
							enter_key = 0;
							name_msg->show();
							textinput->show();
							set_state(GET_SCORE_NAME); // vai para tela de digitar nome
							#if _WIN32 || _WIN64 || __MINGW32__
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
							// é preciso redefinir a fonte
							if (CWriter::instance()->set_font(path, 80) == 0)
								throw "CGameScreen: não conseguiu abrir fonte\n";
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
						window.show_child(false);
						highscore.show(false);
						player->score.set_score(0);
						any_key = 0;
						enter_key = 0;
						title.reset();
						set_state(TITLE_SCREEN); // vai para tela de título
					}
					break;

				case GET_SCORE_NAME: // tela de digitar nome para high score
					if (enter_key)
					{
						SPlayerScore s;
						
						if (textinput->get_str().size() == 0)
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
					
				default:
					set_state(INTRODUCTION);
					break;
			}
			
			return get_state();
		}
};

#endif

