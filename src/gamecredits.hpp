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
			
			CLabel * g = new CLabel("GRAPHICS", (SDL_Color){255,0,0,0});
			widget.add_child(g);
			CLabel * gg = new CLabel("GUSTAVO MEDEIROS", (SDL_Color){255,255,0,0});
			widget.add_child(gg);
			CLabel * p = new CLabel("PROGRAMMING", (SDL_Color){255,0,0,0});
			widget.add_child(p);
			CLabel * ps = new CLabel("SAMUEL LEONARDO", (SDL_Color){255,255,0,0});
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
			
			#ifndef USE_SDL2
				anim.add_frame((SDL_Rect){0,0,0,0}, 250);
			#else
				anim.add_frame(NULL, (SDL_Rect){0,0,0,0}, 250);
			#endif
			set_state(ACTIVE_CREDITS);
		}
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen )
			{
				SDL_FillRect(screen, NULL, 0x0);
				widget.draw(screen);
			}
		#else
			void draw ( SDL_Renderer * renderer )
			{
				SDL_SetRenderDrawColor(renderer, 0,0,0,0xFF);
				SDL_RenderFillRect(renderer, NULL);
				widget.draw(renderer);
			}
		#endif
		
		void reset (  )
		{
			anim.reset();
			set_state(ACTIVE_CREDITS);
		}
		
		int update (  )
		{
			if (anim.update() == 3)
				set_state(INACTIVE_CREDITS);
			return get_state();
		}
};

#endif

