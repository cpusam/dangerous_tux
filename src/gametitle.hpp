#ifndef GAMETITLE_HPP
#define GAMETITLE_HPP

class CGameTitle: public CStateMachine
{
	protected:
		SDL_Surface * background;
		SDL_Surface * title_name;
		CAnimation tux_rocket;
		CLabel * press_enter;
		CLabel * version;
		CAnimation p_enter;
		
		SVect tr_pos;
		SVect tn_pos;
		SVect tn_init;
		SVect tn_final;
		float tn_vel;
	
	public:
		CGameTitle (  )
		{
			background = 0;
			title_name = 0;
			tux_rocket.surface = 0;
			
			press_enter = 0;
			version = 0;
		}
		
		~CGameTitle (  )
		{
			if (background)
				SDL_FreeSurface(background);
		
			if (title_name)
				SDL_FreeSurface(title_name);
		
			if (tux_rocket.surface)
				SDL_FreeSurface(tux_rocket.surface);
		}
	
		void load (  )
		{
			#if _WIN32 || _WIN64 || __MINGW32__
				char path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
			#else
				char path[1024];
			#endif
		
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\gametitle_BG.jpg", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\gametitle_BG.jpg", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/gametitle_BG.jpg");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/gametitle_BG.jpg", PREFIX);
				#endif
			#endif
			background = optimize_surface_alpha(IMG_Load(path));
		
			if (!background)
				throw "CGameTitle: não conseguiu carregar imagem de background\n";
		
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\gametitle_name.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\gametitle_name.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/gametitle_name.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/gametitle_name.png", PREFIX);
				#endif
			#endif
			title_name = optimize_surface_alpha(IMG_Load(path));
		
			if (!title_name)
				throw "CGameTitle: não conseguiu carregar imagem de título\n";
		
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\tux_rocket_sprites.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\tux_rocket_sprites.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/tux_rocket_sprites.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/tux_rocket_sprites.png", PREFIX);
				#endif
			#endif
			tux_rocket.surface = optimize_surface_alpha(IMG_Load(path));

			if (!tux_rocket.surface)
				throw "CGameTitle: não conseguiu carregar imagem do tux no foguete\n";

		
			tn_final = SVect(34,60);
			tn_init = tn_pos = SVect(-title_name->w, tn_final.y);
			tn_vel = 10;
			tr_pos.x = 600;
			tux_rocket.add_frame((SDL_Rect){0,0,319,599}, 3);
			tux_rocket.add_frame((SDL_Rect){0,599,319,599}, 3);
			tux_rocket.add_frame((SDL_Rect){0,599*2,319,599}, 3);
			tux_rocket.add_frame((SDL_Rect){0,599*3,319,599}, 3);
			tux_rocket.add_frame((SDL_Rect){0,599*4,319,599}, 3);
			tux_rocket.add_frame((SDL_Rect){0,599*5,319,599}, 3);
			
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
			if (!CWriter::instance()->set_font(path, 70))
				throw "CGameTitle: não foi possível carregar fonte\n";
			
			press_enter = new CLabel("PRESS ENTER!", (SDL_Color){0,0,0,0});
			press_enter->set_pos(SVect(188,439));
			version = new CLabel("BETA VERSION - 2014", (SDL_Color){255,255,0,0});
			version->set_pos(SVect((960 - version->get_surface()->w)/2, 624 - version->get_surface()->h));
			p_enter.add_frame((SDL_Rect){0,0,0,0}, 25);
			p_enter.add_frame((SDL_Rect){0,0,0,0}, 25);
		}

		void reset (  )
		{
			tn_pos = tn_init;
			tux_rocket.reset();
		}
	
		int update (  )
		{
			tn_pos.x += tn_vel;
			if (tn_pos.x > tn_final.x)
				tn_pos = tn_final;
			
			tux_rocket.update();
			
			p_enter.update();
			if (p_enter.get_index() == 1)
				press_enter->show(false);
			else
				press_enter->show();
			
			return get_state();
		}
	
		void draw ( CCamera * cam, SDL_Surface * screen )
		{
			SDL_BlitSurface(background, NULL, screen, NULL);
		
			SDL_Rect d;
		
			d.x = tn_pos.x;
			d.y = tn_pos.y;
			d.w = title_name->w;
			d.h = title_name->h;
		
			SDL_BlitSurface(title_name, NULL, screen, &d);
		
			tux_rocket.draw(tr_pos.x, tr_pos.y, screen);
			press_enter->draw(screen);
			version->draw(screen);
		}
};

#endif
