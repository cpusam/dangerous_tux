#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

class CGameOver: public CStateMachine
{
	protected:
		SDL_Surface * background;
		SDL_Surface * title;
		CAnimation stars;
		
		SVect t_pos;
		SVect s_pos;
	
	public:
		CGameOver (  )
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
					sprintf(path, "%s\\images\\gameover_BG.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\gameover_BG.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/gameover_BG.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/gameover_BG.png", PREFIX);
				#endif
			#endif
			background = optimize_surface_alpha(IMG_Load(path));
			
			if (!background)
				throw "SGameOver: não conseguiu abrir gameover_BG\n";
			
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\gameover_title.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\gameover_title.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/gameover_title.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/gameover_title.png", PREFIX);
				#endif
			#endif
			title = optimize_surface_alpha(IMG_Load(path));
			
			if (!title)
				throw "SGameOver: não conseguiu abrir gameover_title\n";
				
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\gameover_stars.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\gameover_stars.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/gameover_stars.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/gameover_stars.png", PREFIX);
				#endif
			#endif
			stars.surface = optimize_surface_alpha(IMG_Load(path));
			
			if (!stars.surface)
				throw "SGameOver: não conseguiu abrir gameover_stars\n";
			
			t_pos.x = (background->w - title->w)/2.0f;
			s_pos = SVect(254,220);
			
			stars.add_frame((SDL_Rect){0,0,296,184}, 3);
			stars.add_frame((SDL_Rect){0,184,296,184}, 3);
			stars.add_frame((SDL_Rect){0,184*2,296,184}, 3);
			stars.add_frame((SDL_Rect){0,184*3,296,184}, 3);
			stars.add_frame((SDL_Rect){0,184*4,296,184}, 3);
			stars.add_frame((SDL_Rect){0,184*5,296,184}, 3);
			stars.add_frame((SDL_Rect){0,184*6,296,184}, 3);
		}
		
		~CGameOver (  )
		{
			if (background)
				SDL_FreeSurface(background);
			
			if (title)
				SDL_FreeSurface(title);
			
			if (stars.surface)
				SDL_FreeSurface(stars.surface);
		}
		
		void reset (  )
		{
			stars.reset();
		}
		
		int update (  )
		{
			stars.update();
			return get_state();
		}
		
		void draw ( CCamera * cam, SDL_Surface * screen )
		{
			SDL_Rect d;
			
			SDL_BlitSurface(background, NULL, screen, NULL);
			
			d.x = t_pos.x;
			d.y = t_pos.y;
			d.w = title->w;
			d.h = title->h;
			SDL_BlitSurface(title, NULL, screen, &d);
			
			stars.draw(s_pos.x, s_pos.y, screen);
		}
};

#endif
