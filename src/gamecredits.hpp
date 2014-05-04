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
		CWidget widget;
		CAnimation anim; // apenas para contar o tempo
		CAnimation tux_anim;
		SVect tux_pos;
		
	public:
		#ifndef USE_SDL2
			CGameCredits (  )
		#else
			CGameCredits ( SDL_Renderer * r )
		#endif
		{
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
			
			#if _WIN32 || _WIN64 || __MINGW32__
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
			
			#ifndef USE_SDL2
				anim.add_frame((SDL_Rect){0,0,0,0}, 250);
				
				tux_anim.surface = optimize_surface_alpha(IMG_Load(path));
				if (!tux_anim.surface)
					throw "CGameCredits: não foi possivel carregar tux_walk.png\n";
				
				tux_anim.add_frame((SDL_Rect){0,    0,214,234}, 3);
				tux_anim.add_frame((SDL_Rect){0,  234,214,234}, 5); // meio
				tux_anim.add_frame((SDL_Rect){0,2*234,214,234}, 3);
				tux_pos.x = widget.get_pos().x - tux_anim.surface->w/2;
			#else
				anim.add_frame(NULL, (SDL_Rect){0,0,0,0}, 250);
				
				SDL_Texture * aux = SDL_CreateTextureFromSurface(r, IMG_Load(path));
				if (!aux)
					throw "CGameCredits: não foi possivel carregar tux_walk.png\n";
				
				tux_anim.add_frame(aux, (SDL_Rect){0,    0,214,234}, 3);
				tux_anim.add_frame(aux, (SDL_Rect){0,  234,214,234}, 5); // meio
				tux_anim.add_frame(aux, (SDL_Rect){0,2*234,214,234}, 3);
				tux_anim.add_frame(aux, (SDL_Rect){0,  234,214,234}, 5); // meio
				tux_pos.x = widget.get_pos().x - texture_width(aux)/2;
			#endif
			tux_pos.y = 624 - 234;
			widget.set_pos(SVect(widget.get_pos().x, widget.get_pos().y - 234/2));
			set_state(ACTIVE_CREDITS);
		}
		
		~CGameCredits (  )
		{
			CWidget * w = widget.get_child(0);
			for (int i = 0; w; i++, w = widget.get_child(i))
				delete w;
			
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
				widget.draw(screen);
				tux_anim.draw(tux_pos.x, tux_pos.y, screen);
			}
		#else
			void draw ( SDL_Renderer * renderer )
			{
				SDL_SetRenderDrawColor(renderer, 0x00,0xc6,0xff,0xFF);
				SDL_RenderFillRect(renderer, NULL);
				widget.draw(renderer);
				tux_anim.draw(tux_pos.x, tux_pos.y, renderer);
			}
		#endif
		
		void reset (  )
		{
			anim.reset();
			tux_anim.reset();
			set_state(ACTIVE_CREDITS);
		}
		
		int update (  )
		{
			tux_anim.update();
			if (anim.update() == 3)
				set_state(INACTIVE_CREDITS);
			return get_state();
		}
};

#endif

