#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

class CGameOver: public CStateMachine
{
	protected:
		#ifndef USE_SDL2
			SDL_Surface * background;
			SDL_Surface * title;
		#else
			SDL_Texture * background;
			SDL_Texture * title;
		#endif
		CAnimation stars;
		
		SVect t_pos;
		SVect s_pos;
	
	public:
		#ifndef USE_SDL2
			CGameOver (  )
		#else
			CGameOver ( SDL_Renderer * r )
		#endif
		{
			#if USE_SDL2
				SDL_Texture * texture = 0;
				SDL_Surface * aux = 0;
			#endif
			
			#if _WIN32 || _WIN64
				char path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
			#else
				char path[1024];
			#endif
			
			#if _WIN32 || _WIN64
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
			
			#ifndef USE_SDL2
				background = optimize_surface_alpha(IMG_Load(path));
			#else
				aux = IMG_Load(path);
				background = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
			#endif
			
			if (!background)
				throw "SGameOver: não conseguiu abrir gameover_BG\n";
			
			#if _WIN32 || _WIN64
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
			
			#ifndef USE_SDL2
				title = optimize_surface_alpha(IMG_Load(path));
			#else
				aux = IMG_Load(path);
				title = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
			#endif
			
			if (!title)
				throw "SGameOver: não conseguiu abrir gameover_title\n";
				
			#if _WIN32 || _WIN64
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
			
			#ifndef USE_SDL2
				stars.surface = optimize_surface_alpha(IMG_Load(path));
				if (!stars.surface)
					throw "SGameOver: não conseguiu abrir gameover_stars\n";
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "SGameOver: não conseguiu abrir gameover_stars\n";
			#endif

			#ifndef USE_SDL2
				t_pos.x = (background->w - title->w)/2.0f;
				stars.add_frame((SDL_Rect){0,0,296,184}, 3);
				stars.add_frame((SDL_Rect){0,184,296,184}, 3);
				stars.add_frame((SDL_Rect){0,184*2,296,184}, 3);
				stars.add_frame((SDL_Rect){0,184*3,296,184}, 3);
				stars.add_frame((SDL_Rect){0,184*4,296,184}, 3);
				stars.add_frame((SDL_Rect){0,184*5,296,184}, 3);
				stars.add_frame((SDL_Rect){0,184*6,296,184}, 3);
			#else
				t_pos.x = (texture_width(background) - texture_width(title))/2.0f;
				stars.add_frame(texture, (SDL_Rect){0,0,296,184}, 3);
				stars.add_frame(texture, (SDL_Rect){0,184,296,184}, 3);
				stars.add_frame(texture, (SDL_Rect){0,184*2,296,184}, 3);
				stars.add_frame(texture, (SDL_Rect){0,184*3,296,184}, 3);
				stars.add_frame(texture, (SDL_Rect){0,184*4,296,184}, 3);
				stars.add_frame(texture, (SDL_Rect){0,184*5,296,184}, 3);
				stars.add_frame(texture, (SDL_Rect){0,184*6,296,184}, 3);
			#endif
			s_pos = SVect(254,220);
		}
		
		~CGameOver (  )
		{
			#ifndef USE_SDL2
				if (background)
					SDL_FreeSurface(background);
			
				if (title)
					SDL_FreeSurface(title);
			
				if (stars.surface)
					SDL_FreeSurface(stars.surface);
			#else
				if (background)
					SDL_DestroyTexture(background);
			
				if (title)
					SDL_DestroyTexture(title);
			
				stars.destroy_textures();
			#endif
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
		
		#ifndef USE_SDL2
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
		#else
			void draw ( CCamera * cam, SDL_Renderer * renderer )
			{
				SDL_Rect d;
			
				SDL_RenderCopy(renderer, background, NULL, NULL);
			
				d.x = t_pos.x;
				d.y = t_pos.y;
				d.w = texture_width(title);
				d.h = texture_height(title);
				SDL_RenderCopy(renderer, title, NULL, &d);
			
				stars.draw(s_pos.x, s_pos.y, renderer);
			}
		#endif
};

#endif
