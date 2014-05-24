#ifndef GAMECREDITS_HPP
#define GAMECREDITS_HPP

enum EGameCreditsState
{
	INACTIVE_CREDITS,
	ACTIVE_CREDITS
};


class CGameCredits: public CStateMachine
{
	protected:
		//CWidget widget;
		CAnimation anim; // apenas para contar o tempo
		CAnimation tux_anim;
		SVect tux_pos;
		SVect bg_pos;
		CBackground bg;
		CCamera * cam;
		int chosed;
		
	public:
		#ifndef USE_SDL2
			CGameCredits (  )
		#else
			CGameCredits ( SDL_Renderer * r )
		#endif
		{
			#if _WIN32 || _WIN64
				char path[FILENAME_MAX], bg_path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
			#else
				char path[1024], bg_path[1024];
			#endif
		
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
			
			if (!CWriter::instance()->set_font(path, 100))
				throw "CGameCredits: não foi possível carregar font\n";

			#if USE_SDL2
				CWriter::instance()->set_renderer(r);
			#endif
			
			char s[4][32] =
			{
				{71,82,65,80,72,73,67,83},
				{71,85,83,84,65,86,79,32,77,69,68,69,73,82,79,83},
				{80,82,79,71,82,65,77,77,73,78,71},
				{83,65,77,85,69,76,32,76,69,79,78,65,82,68,79},
			};
			
			/*
			CLabel * g = new CLabel(s[0], (SDL_Color){255,0,0,0});
			widget.add_child(g);
			CLabel * gg = new CLabel(s[1], (SDL_Color){255,255,0,0});
			widget.add_child(gg);
			CLabel * p = new CLabel(s[2], (SDL_Color){255,0,0,0});
			widget.add_child(p);
			CLabel * ps = new CLabel(s[3], (SDL_Color){255,255,0,0});
			widget.add_child(ps);
			
			widget.set_pos(SVect(960/2,624/2));
			#ifndef USE_SDL2
				int h = g->get_surface()->h + gg->get_surface()->h + p->get_surface()->h + ps->get_surface()->h;
				g->set_rel_pos(SVect(-(g->get_surface()->w/2), -h/2));
				gg->set_rel_pos(SVect(-(gg->get_surface()->w/2), g->get_surface()->h + g->get_rel_pos().y));
				p->set_rel_pos(SVect(-(p->get_surface()->w/2), gg->get_surface()->h + gg->get_rel_pos().y));
				ps->set_rel_pos(SVect(-(ps->get_surface()->w/2), p->get_surface()->h + p->get_rel_pos().y));
			#else
				int h = g->get_texture_height() + gg->get_texture_height() + p->get_texture_height() + ps->get_texture_height();
				g->set_rel_pos(SVect(-(g->get_texture_width()/2), -h/2));
				gg->set_rel_pos(SVect(-(gg->get_texture_width()/2), g->get_texture_height() + g->get_rel_pos().y));
				p->set_rel_pos(SVect(-(p->get_texture_width()/2), gg->get_texture_height() + gg->get_rel_pos().y));
				ps->set_rel_pos(SVect(-(ps->get_texture_width()/2), p->get_texture_height() + p->get_rel_pos().y));
			#endif
			*/
			
			#if _WIN32 || _WIN64
				#ifndef PREFIX
						sprintf(path, "%s\\images\\tux_walk.png", p2);
					#else
						sprintf(path, "%s\\dangeroustux\\images\\tux_walk.png", PREFIX);
					#endif
				#else
					#ifndef PREFIX
						sprintf(path, "./images/tux_walk.png");
					#else
						sprintf(path, "%s/share/games/dangeroustux/images/tux_walk.png", PREFIX);
				#endif
			#endif
			
			#if _WIN32 || _WIN64
					#ifndef PREFIX
							sprintf(bg_path, "%s\\images\\credits_BG.png", p2);
						#else
							sprintf(bg_path, "%s\\dangeroustux\\images\\credits_BG.png", PREFIX);
						#endif
					#else
						#ifndef PREFIX
							sprintf(bg_path, "./images/credits_BG.png");
						#else
							sprintf(bg_path, "%s/share/games/dangeroustux/images/credits_BG.png", PREFIX);
					#endif
				#endif
			
			#ifndef USE_SDL2
				anim.add_frame((SDL_Rect){0,0,0,0}, 250);
				
				tux_anim.surface = optimize_surface_alpha(IMG_Load(path));
				if (!tux_anim.surface)
					throw "CGameCredits: não foi possivel carregar tux_walk.png\n";
				
				tux_anim.add_frame((SDL_Rect){0,    0,214,234}, 3);
				tux_anim.add_frame((SDL_Rect){0,  234,214,234}, 5); // meio
				tux_anim.add_frame((SDL_Rect){0,2*234,214,234}, 3);
				tux_anim.add_frame((SDL_Rect){0,  234,214,234}, 5); // meio
				//tux_pos.x = widget.get_pos().x - tux_anim.surface->w/2;
				tux_pos.x = (960 - tux_anim.surface->w)/2;
				
				if (!bg.set_surface(optimize_surface_alpha(IMG_Load(bg_path))))
					throw "CGameCredits: não foi possível carregar credits_BG.png\n";
			#else
				anim.add_frame(NULL, (SDL_Rect){0,0,0,0}, 250);
				
				SDL_Surface * aux = IMG_Load(path);
				SDL_Texture * texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "CGameCredits: não foi possivel carregar tux_walk.png\n";
				
				tux_anim.add_frame(texture, (SDL_Rect){0,    0,214,234}, 3);
				tux_anim.add_frame(texture, (SDL_Rect){0,  234,214,234}, 5); // meio
				tux_anim.add_frame(texture, (SDL_Rect){0,2*234,214,234}, 3);
				tux_anim.add_frame(texture, (SDL_Rect){0,  234,214,234}, 5); // meio
				//tux_pos.x = widget.get_pos().x - texture_width(texture)/2;
				tux_pos.x = (960 - texture_width(texture))/2
				
				aux = IMG_Load(bg_path);
				if (!bg.set_texture(SDL_CreateTextureFromSurface(r, aux)))
					throw "CGameCredits: não foi possível carregar credits_BG.png\n";
				SDL_FreeSurface(aux);
			#endif
			//widget.set_pos(SVect(960/2, 358/2));
			tux_pos.y = 358;
			
			#ifndef USE_SDL2
				cam = new CCamera((SDL_Rect){0,0,bg.get_surface()->w,bg.get_surface()->h}, (SDL_Rect){0,0,2000*bg.get_surface()->w,bg.get_surface()->h});
			#else
				cam = new CCamera((SDL_Rect){0,0,texture_width(bg.get_texture()),texture_height(bg.get_texture())}, (SDL_Rect){0,0,2000*texture_width(bg.get_texture()),texture_height(bg.get_texture())});
			#endif
			set_state(ACTIVE_CREDITS);
		}
		
		~CGameCredits (  )
		{
		/*
			CWidget * w = widget.get_child(0);
			for (int i = 0; w; i++, w = widget.get_child(i))
				delete w;
		*/

			delete cam;
			#ifndef USE_SDL2
				SDL_FreeSurface(tux_anim.surface);
			#else
				tux_anim.destroy_textures();
			#endif
		}
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen )
			{
				SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00,0xc6,0xff));
				bg.draw_hor(cam, screen);
				tux_anim.draw(tux_pos.x, tux_pos.y, screen);

				//widget.draw(screen);
			}
		#else
			void draw ( SDL_Renderer * renderer )
			{
				SDL_SetRenderDrawColor(renderer, 0x00,0xc6,0xff,0xFF);
				SDL_RenderFillRect(renderer, NULL);
				
				bg.draw_hor(cam, renderer);
				tux_anim.draw(tux_pos.x, tux_pos.y, renderer);

				//widget.draw(renderer);
			}
		#endif
		
		void reset (  )
		{
			bg_pos.zero();
			cam->set_position(SVect());
		
			anim.reset();
			tux_anim.reset();
			set_state(ACTIVE_CREDITS);
		}
		
		int update (  )
		{
			bg_pos.x += 15.0f;
			cam->set_position(bg_pos);
			
			tux_anim.update();
			if (anim.update() == 3)
				set_state(INACTIVE_CREDITS);
			
			return get_state();
		}
};

#endif

