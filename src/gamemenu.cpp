#include "gamemenu.hpp"

COption::COption ( std::string s, SDL_Color c[3] )
{
	set_id(s);
	
	label[0] = new GuiLabel(s, c[0]);
	label[1] = new GuiLabel(s, c[1]);
	label[2] = new GuiLabel(s, c[2]);
	
	add_child(label[0]);
	add_child(label[1]);
	add_child(label[2]);
	set_dim(label[0]->get_dim());
	set_state(1);
}

COption::~COption (  )
{
	clear_child();
	
	delete label[0];
	delete label[1];
	delete label[2];
}

int COption::update (  )
{
	switch (get_state())
	{
		case 1: // normal
			label[0]->show(true);
			label[1]->show(false);
			label[2]->show(false);
			break;
			
		case 2: // selecionado
			label[0]->show(false);
			label[1]->show(true);
			label[2]->show(false);
			break;
			
		case 3: // pressionado
			label[0]->show(false);
			label[1]->show(false);
			label[2]->show(true);
			break;
			
		case 4: // solto depois de pressionado
			set_state(5);
			break;
		
		case 5:
			set_state(1);
			break;
			
		default:
			label[0]->show(true);
			label[1]->show(false);
			label[2]->show(false);
			set_state(1);
			break;
	}
	
	return get_state();
}


void COption::draw ( SDL_Renderer * renderer )
{
	switch (get_state())
	{
		case 1:
			label[0]->draw(renderer);
			break;
		case 2:
			label[1]->draw(renderer);
			break;
		case 3:
			label[2]->draw(renderer);
			break;
		default:
			label[0]->draw(renderer);
			break;
	}
}

////////////////////////////////////////////////////////////////
CGameOptions::CGameOptions ( CSaveGame * s[3] )
{
	save[0] = s[0];
	save[1] = s[1];
	save[2] = s[2];
	curr_save = save[0];
	
	/*
		NEW GAME
		NEW GAME
		NEW GAME
		CREDITS
		  EXIT
	*/
	option.resize(5, 0);
	
	color[0] = (SDL_Color){255,255,  0,255}, // normal
	color[1] = (SDL_Color){  0,  0,255,255}, // selecionado
	color[2] = (SDL_Color){255,  0,  0,255}; // apertando enter
	reset_options();
	
	show(false);
	set_state(INACTIVE_GAMEOPTIONS);
}

CGameOptions::~CGameOptions (  )
{
	clear_child();
	
	for (int i(0); i < option.size(); i++)
		delete option[i];
}

void CGameOptions::set_color ( SDL_Color c[3] )
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
}

COption * CGameOptions::get_chose_option (  )
{
	return chose_option;
}

std::vector <COption *> CGameOptions::get_option (  )
{
	return option;
}

COption * CGameOptions::get_curr_option (  )
{
	return curr_option;
}

CSaveGame * CGameOptions::get_curr_save (  )
{
	return curr_save;
}

void CGameOptions::reset (  )
{
	show();
	reset_options();
	set_state(ACTIVE_GAMEOPTIONS);
}

	void CGameOptions::reset_options (  )
	{
		#if _WIN32 || _WIN64
			char path[FILENAME_MAX];
			char p2[FILENAME_MAX];
			_getcwd(p2, sizeof(p2));
			#ifndef PREFIX
				sprintf(path, "%s\\fonts\\BubblegumSans-Regular.otf", p2);
			#else
				sprintf(path, "%s\\dangeroustux\\fonts\\BubblegumSans-Regular.otf", PREFIX);
			#endif
		#else
			char path[1024];
			#ifndef PREFIX
				sprintf(path, "./fonts/BubblegumSans-Regular.otf");
			#else
				sprintf(path, "%s/share/games/dangeroustux/fonts/BubblegumSans-Regular.otf", PREFIX);
			#endif
		#endif

		if (Writer::instance()->load_font(path, path, 80) == 0)
			throw "CGameOptions: não foi possí­vel carregar fonte\n";
	
		chose_option = 0;
	
		for (int i(0); i < option.size(); i++)
			if (option[i])
			{
				delete option[i];
				option[i] = 0;
			}

		SSaveData data;
		char str[256];
		int level = 0;

		for (int i(0); i < 3; i++)
		{
			if (save[i]->load())
			{
				data = save[i]->get_data();
				sscanf(data.curr_level, "%d", &level);
				sprintf(str, "LEVEL: %d - TUXES: %s", ++level, data.lives);
			}
			else
			{
				sprintf(str, "NEW GAME");
			}

			if (option[i])
				delete option[i];

			option[i] = new COption(str, color);
		}
	
		if (!option[3])
			option[3] = new COption("CREDITS", color);
		option[3]->set_state(1);

		if (!option[4])
			option[4] = new COption("EXIT", color);
		option[4]->set_state(1);

		curr_option = option[0];
		curr_option->set_state(2);

		clear_child();
		dim.h = 0;
		dim.w = 0;
		for (int i(0); i < option.size(); i++)
		{
			add_child(option[i]);
			dim.h += option[i]->get_dim().h;
			if (option[i]->get_dim().w > dim.w)
				dim.w = option[i]->get_dim().w;
		}
		set_pos(Vect(960/2, 624/2));
	
		for (int i(0); i < option.size(); i++)
			if (i > 0)
				option[i]->set_rel_pos(Vect(-option[i]->get_dim().w/2,option[i]->get_dim().h + option[i - 1]->get_rel_pos().y));
			else
				option[i]->set_rel_pos(Vect(-option[i]->get_dim().w/2,-dim.h/2));
		
		set_pos(Vect(370, pos.y));
	}

	void CGameOptions::input ( SDL_Event & event )
	{
		int i;
		if (event.type == SDL_KEYDOWN && get_state() != INACTIVE_GAMEOPTIONS)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_DOWN:
					for (i = 0; i < option.size(); i++)
						if (curr_option == option[i])
						{
							curr_option->set_state(1);
							if (i + 1 == option.size())
								curr_option = option[0];
							else
								curr_option = option[i + 1];
							curr_option->set_state(2);
							break;
						}
					break;
	
				case SDLK_UP:
					for (i = 0; i < option.size(); i++)
						if (curr_option == option[i])
						{
							curr_option->set_state(1);
							if (i - 1 < 0)
								curr_option = option.back();
							else
								curr_option = option[i - 1];
							curr_option->set_state(2);
							break;
						}
					break;
	
				case SDLK_DELETE: // apaga o profile antigo
					#if _WIN32 || _WIN64
						char path[FILENAME_MAX];
						char p2[FILENAME_MAX];
						_getcwd(p2, sizeof(p2));
						#ifndef PREFIX
							sprintf(path, "%s\\fonts\\BubblegumSans-Regular.otf", p2);
						#else
							sprintf(path, "%s\\dangeroustux\\fonts\\BubblegumSans-Regular.otf", PREFIX);
						#endif
					#else
						char path[1024];
						#ifndef PREFIX
							sprintf(path, "./fonts/BubblegumSans-Regular.otf");
						#else
							sprintf(path, "%s/share/games/dangeroustux/fonts/BubblegumSans-Regular.otf", PREFIX);
						#endif
					#endif

					if (Writer::instance()->load_font(path, path, 80) == 0)
						throw "CGameOptions: não foi possí­vel carregar fonte\n";
			
					for (i = 0; i < 3; i++)
						if (curr_option == option[i] && save[i]->was_loaded())
						{
							save[i]->erase_profile();
							rem_child(option[i]);
							delete option[i];
							option[i] = new COption("NEW GAME", color);
							add_child(option[i]);
							option[i]->set_rel_pos(Vect(-option[i]->get_dim().w/2, -dim.h/2));
							curr_option = option[i];
							curr_option->set_state(2);
							break;
						}
					break;
	
				case SDLK_RETURN:
				case SDLK_KP_ENTER:
					if (get_state() != INACTIVE_GAMEOPTIONS)
					{
						for (i = 0; i < option.size(); i++)
							if (curr_option == option[i])
							{
								curr_option->set_state(3); // vai para pressionado
								break;
							}
					}
				break;

				default:
					break;
			}
		}
		else if (event.type == SDL_KEYUP && get_state() != INACTIVE_GAMEOPTIONS)
		{
			if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
			{
				for (i = 0; i < option.size(); i++)
					if (curr_option == option[i] && option[i]->get_state() == 3)
					{
						curr_save = save[i];
						chose_option = curr_option;
						set_state(INACTIVE_GAMEOPTIONS);
						break;
					}
			}
		}
	}

	int CGameOptions::update (  )
	{
		switch (get_state())
		{
			case ACTIVE_GAMEOPTIONS:
				for (int i(0); i < option.size(); i++)
					option[i]->update();
				break;
			
			case INACTIVE_GAMEOPTIONS:
				show(false);
				break;
			
			default:
				set_state(ACTIVE_GAMEOPTIONS);
				break;
		}

		return get_state();
	}

	void CGameOptions::draw ( SDL_Renderer * renderer )
	{
		if (is_visible())
			for (int i(0); i < option.size(); i++)
				option[i]->draw(renderer);
	}

/////////////////////////////////////////////////////////////////

CGameMenu::CGameMenu ( CSaveGame *save[3] ): options(save)
{
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path, "%s\\fonts\\BubblegumSans-Regular.otf", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\fonts\\BubblegumSans-Regular.otf", PREFIX);
		#endif
	#else
		char path[1024];
		#ifndef PREFIX
			sprintf(path, "./fonts/BubblegumSans-Regular.otf");
		#else
			sprintf(path, "%s/share/games/dangeroustux/fonts/BubblegumSans-Regular.otf", PREFIX);
		#endif
	#endif

	if (Writer::instance()->load_font(path, path, 80) == 0)
		throw "CGameMenu: não foi possí­vel carregar fonte\n";
	
	header = new GuiLabel("CHOOSE AN OPTION:", (SDL_Color){0,0,0,255});
	header->set_pos(Vect(options.get_pos().x - header->get_dim().w/2, options.get_child(0)->get_pos().y - header->get_dim().h));
	
	chose_option = 0;
	set_state(INIT_GAMEMENU);
}

CGameMenu::~CGameMenu (  )
{
	delete header;
}

void CGameMenu::input ( SDL_Event & event )
{
	if (get_state() == RUNNING_GAMEMENU)
		options.input(event);
}

void CGameMenu::reset (  )
{
	set_state(INIT_GAMEMENU);
}

void CGameMenu::draw ( SDL_Renderer * renderer )
{
	if (get_state() == INIT_GAMEMENU)
		return;

	header->draw(renderer);
	options.draw(renderer);
}

int CGameMenu::update (  )
{
	switch (get_state())
	{
		case INIT_GAMEMENU:
			header->show();
			options.reset();
			options.show();
			set_state(RUNNING_GAMEMENU);
			break;

		case RUNNING_GAMEMENU:
			options.update();
		
			if (options.get_state() == INACTIVE_GAMEOPTIONS)
				set_state(CHOSE_OPTION);
			break;
		
		case CHOSE_OPTION:
			set_state(END_GAMEMENU);
			break;
	}
	
	return get_state();
}

