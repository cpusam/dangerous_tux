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
		CGameCredits (  )
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
			
			CLabel * g = new CLabel("GRAPHICS", (SDL_Color){255,0,0,0});
			widget.add_child(g);
			CLabel * gg = new CLabel("GUSTAVO MEDEIROS", (SDL_Color){255,255,0,0});
			widget.add_child(gg);
			CLabel * p = new CLabel("PROGRAMMING", (SDL_Color){255,0,0,0});
			widget.add_child(p);
			CLabel * ps = new CLabel("SAMUEL LEONARDO", (SDL_Color){255,255,0,0});
			widget.add_child(ps);
			
			int h = g->get_surface()->h + gg->get_surface()->h + p->get_surface()->h + ps->get_surface()->h;
			g->set_rel_pos(SVect(-(g->get_surface()->w/2), -h/2));
			gg->set_rel_pos(SVect(-(gg->get_surface()->w/2), g->get_surface()->h + g->get_rel_pos().y));
			p->set_rel_pos(SVect(-(p->get_surface()->w/2), gg->get_surface()->h + gg->get_rel_pos().y));
			ps->set_rel_pos(SVect(-(ps->get_surface()->w/2), p->get_surface()->h + p->get_rel_pos().y));

			widget.set_pos(SVect(960/2, 624/2));
			anim.add_frame((SDL_Rect){0,0,0,0}, 250);
			set_state(ACTIVE_CREDITS);
		}
		
		void draw ( SDL_Surface * screen )
		{
			SDL_FillRect(screen, NULL, 0x0);
			widget.draw(screen);
		}
		
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
