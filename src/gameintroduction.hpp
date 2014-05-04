#ifndef GAMEINTRODUCTION_HPP
#define GAMEINTRODUCTION_HPP

enum EGameIntroductionState
{
	PARALLEL_UNIVERSE,
	LINUS_TYPING,
	ENTER_KEYDOWN,
	KERNEL_HACKED,
	LINUS_FACE,
	LINUS_TALKING,
	TUX_TALKING,
	TUX_OUT,
	INACTIVE_INTRODUCTION
};

class CGameIntroduction: public CStateMachine
{
	protected:
		vector <CAnimation> anim;
		#ifndef USE_SDL2
			SDL_Surface * browser_bg;
			SDL_Surface * keyboard_bg;
			SDL_Surface * kernel_site;
		#else
			SDL_Texture * browser_bg;
			SDL_Texture * keyboard_bg;
			SDL_Texture * kernel_site;
		#endif
		CLabel * parallel; // frase: "In a parallel universe..."
		int pos_hand; // posição da mão
		int init_hand; // posição inicial da mão
		int final_hand; // posição final da mão
		int vel_hand; // velocidade de movimento da mão
	
	public:
		#ifndef USE_SDL2
			CGameIntroduction (  )
		#else
			CGameIntroduction ( SDL_Renderer * r )
		#endif
		{
			#if USE_SDL2
				SDL_Texture * texture = 0;
				SDL_Surface * aux = 0;
			#endif
			
			#if _WIN32 || _WIN64 || __MINGW32__
				char path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
			#else
				char path[1024];
			#endif
			
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\scene1_BG.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene1_BG.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/scene1_BG.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene1_BG.png", PREFIX);
				#endif
			#endif
			
			#ifndef USE_SDL2
				browser_bg = optimize_surface_alpha(IMG_Load(path));
			#else
				aux = IMG_Load(path);
				browser_bg = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
			#endif
			if (!browser_bg)
				throw "CGameIntroduction: não foi possível carregar scene1_BG.png\n";
			
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\scene2_BG.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene2_BG.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/scene2_BG.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene2_BG.png", PREFIX);
				#endif
			#endif
			
			#ifndef USE_SDL2
				keyboard_bg = optimize_surface_alpha(IMG_Load(path));
			#else
				aux = IMG_Load(path);
				keyboard_bg = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
			#endif
			if (!keyboard_bg)
				throw "CGameIntroduction: não foi possível carregar scene2_BG.png\n";
			
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\scene3_BG.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene3_BG.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/scene3_BG.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene3_BG.png", PREFIX);
				#endif
			#endif
			
			#ifndef USE_SDL2
				kernel_site = optimize_surface_alpha(IMG_Load(path));
			#else
				aux = IMG_Load(path);
				kernel_site = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
			#endif
			if (!kernel_site)
				throw "CGameIntroduction: não foi possível carregar scene3_BG.png\n";
			
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

			if (!CWriter::instance()->set_font(path, 80))
				throw "CGameIntroduction: não foi possível carregar font\n";
			
			#if USE_SDL2
				CWriter::instance()->set_renderer(r);
			#endif
			
			parallel = new CLabel("In a parallel universe...", (SDL_Color){255,255,0,0});
			#ifndef USE_SDL2
				parallel->set_pos(SVect((960 - parallel->get_surface()->w)/2, (624 - parallel->get_surface()->h)/2));
			#else
				parallel->set_pos(SVect((960 - parallel->get_texture_width())/2, (624 - parallel->get_texture_height())/2));
			#endif
			/*
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/", PREFIX);
				#endif
			#endif
			
			anim[].surface = optimize_surface_alpha(IMG_Load(path));
			if (!anim[].surface)
				throw "CGameIntroduction: não foi possível carregar \n";
			*/
			
			CSound sound;
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\sounds\\key_press.wav", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\sounds\\key_press.wav", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./sounds/key_press.wav");
				#else
					sprintf(path, "%s/share/games/dangeroustux/sounds/key_press.wav", PREFIX);
				#endif
			#endif
			sound.set_chunk(path);
			CSoundPlayer::instance()->add_sound(sound);
			
			anim.resize(8);
			
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\scene2-Enter.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene2-Enter.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/scene2-Enter.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene2-Enter.png", PREFIX);
				#endif
			#endif
			
			#ifndef USE_SDL2
				anim[0].surface = optimize_surface_alpha(IMG_Load(path));
				if (!anim[0].surface)
					throw "CGameIntroduction: não foi possível carregar scene2-Enter.png\n";
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "CGameIntroduction: não foi possível carregar scene2-Enter.png\n";
				
				anim[0].set_repeat(false); // animação da tecla enter pressionada
				anim[0].add_frame(texture, (SDL_Rect){0,0,270,269}, 1);
				anim[0].add_frame(texture, (SDL_Rect){0,269,270,269}, 2);
				anim[0].add_frame(texture, (SDL_Rect){0,269*2,270,269}, 20);
			#endif
						
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\scene2-Hand.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene2-Hand.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/scene2-Hand.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene2-Hand.png", PREFIX);
				#endif
			#endif
			
			#ifndef USE_SDL2
				anim[1].surface = optimize_surface_alpha(IMG_Load(path));
				if (!anim[1].surface)
					throw "CGameIntroduction: não foi possível carregar scene2-Hand.png\n";
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "CGameIntroduction: não foi possível carregar scene2-Hand.png\n";
				
				anim[1].set_repeat(false); // animação da mão
				anim[1].pause();
				anim[1].add_frame(texture, (SDL_Rect){0,0,698,585}, 15);
				anim[1].add_frame(texture, (SDL_Rect){0,585,698,585}, 15);
			#endif
			
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\scene4.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene4.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/scene4.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene4.png", PREFIX);
				#endif
			#endif
			
			#ifndef USE_SDL2
				anim[3].surface = optimize_surface_alpha(IMG_Load(path));
				if (!anim[3].surface)
					throw "CGameIntroduction: não foi possível carregar scene4.png\n";
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "CGameIntroduction: não foi possível carregar scene4.png\n";
				
				anim[3].set_repeat(false); // animação do rosto de Linus
				anim[3].add_frame(texture, (SDL_Rect){0,0,960,624}, 20);
				anim[3].add_frame(texture, (SDL_Rect){0,624,960,624}, 25);
			#endif
			
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					sprintf(path, "%s\\images\\intro_scene\\scene1-Kernel.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene1-Kernel.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "./images/intro_scene/scene1-Kernel.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene1-Kernel.png", PREFIX);
				#endif
			#endif
			
			#ifndef USE_SDL2
				anim[4].surface = optimize_surface_alpha(IMG_Load(path));
				if (!anim[4].surface)
					throw "CGameIntroduction: não foi possível carregar scene1-Kernel.png\n";
			#else
				aux = IMG_Load(path);
				texture = SDL_CreateTextureFromSurface(r, aux);
				SDL_FreeSurface(aux);
				if (!texture)
					throw "CGameIntroduction: não foi possível carregar scene1-Kernel.png\n";
				
				anim[4].add_frame(texture, (SDL_Rect){0,170*9,717,170}, 20); // animação de digitando
				anim[4].add_frame(texture, (SDL_Rect){0,170*8,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,170*7,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,170*6,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,170*5,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,170*4,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,170*3,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,170*2,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,170,717,170}, 10);
				anim[4].add_frame(texture, (SDL_Rect){0,0,717,170}, 25);
				anim[4].set_repeat(false);
			#endif
			
			#ifndef USE_SDL2
				anim[0].set_repeat(false); // animação da tecla enter pressionada
				anim[0].add_frame((SDL_Rect){0,0,270,269}, 1);
				anim[0].add_frame((SDL_Rect){0,269,270,269}, 2);
				anim[0].add_frame((SDL_Rect){0,269*2,270,269}, 20);

				anim[1].set_repeat(false); // animação da mão
				anim[1].pause();
				anim[1].add_frame((SDL_Rect){0,0,698,585}, 15);
				anim[1].add_frame((SDL_Rect){0,585,698,585}, 15);
			
				anim[2].add_frame((SDL_Rect){0,0,0,0}, 75); // animação do hack do kernel
				anim[2].set_repeat(false);

				anim[3].set_repeat(false); // animação do rosto de Linus
				anim[3].add_frame((SDL_Rect){0,0,960,624}, 20);
				anim[3].add_frame((SDL_Rect){0,624,960,624}, 25);
			
				anim[4].add_frame((SDL_Rect){0,170*9,717,170}, 20); // animação de digitando
				anim[4].add_frame((SDL_Rect){0,170*8,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,170*7,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,170*6,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,170*5,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,170*4,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,170*3,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,170*2,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,170,717,170}, 10);
				anim[4].add_frame((SDL_Rect){0,0,717,170}, 25);
				anim[4].set_repeat(false);
			
				anim[5].add_frame((SDL_Rect){0,0,0,0}, 100); // em um universo paralelo
				anim[5].set_repeat(false);
			#else
				anim[2].add_frame(0, (SDL_Rect){0,0,0,0}, 75); // animação do hack do kernel
				anim[2].set_repeat(false);
				
				anim[5].add_frame(0, (SDL_Rect){0,0,0,0}, 100); // em um universo paralelo
				anim[5].set_repeat(false);
			#endif
			
			#ifndef USE_SDL2
				init_hand = -anim[1].surface->w/2;
				final_hand = -(anim[1].surface->w - 567); 
			#else
				init_hand = -texture_width(anim[1].get_texture(0))/2;
				final_hand = -(texture_width(anim[1].get_texture(0)) - 567); 
			#endif
			pos_hand = init_hand;
			vel_hand = 20;
			reset();
		}
		
		~CGameIntroduction (  )
		{
			#ifndef USE_SDL2
				if (parallel)
					delete parallel;

				if (browser_bg)
					SDL_FreeSurface(browser_bg);
			
				if (keyboard_bg)
					SDL_FreeSurface(keyboard_bg);
			
				for (int i(0); i < anim.size(); i++)
					if (anim[i].surface)
						SDL_FreeSurface(anim[i].surface);
			#else
				if (parallel)
					delete parallel;

				if (browser_bg)
					SDL_DestroyTexture(browser_bg);
			
				if (keyboard_bg)
					SDL_DestroyTexture(keyboard_bg);
			
				for (int i(0); i < anim.size(); i++)
					anim[i].destroy_textures();
			#endif
		}
		
		void reset (  )
		{
			anim[5].reset();
			set_state(PARALLEL_UNIVERSE);
		}
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen )
		#else
			void draw ( SDL_Renderer * renderer )
		#endif
		{
			switch (get_state())
			{
				case PARALLEL_UNIVERSE:
					#ifndef USE_SDL2
						SDL_FillRect(screen, NULL, 0x0);
						parallel->draw(screen);
					#else
						SDL_SetRenderDrawColor(renderer, 0,0,0,255);
						SDL_RenderFillRect(renderer, NULL);
						parallel->draw(renderer);
					#endif
					break;
				case LINUS_TYPING:
					#ifndef USE_SDL2
						SDL_BlitSurface(browser_bg, NULL, screen, NULL);
						anim[4].draw(206,206, screen);
					#else
						SDL_RenderCopy(renderer, browser_bg, NULL, NULL);
						anim[4].draw(206,206, renderer);
					#endif
					break;
					
				case ENTER_KEYDOWN:
					#ifndef USE_SDL2
						SDL_BlitSurface(keyboard_bg, NULL, screen, NULL);

						// tecla enter pressionada
						anim[0].draw(385, 80, screen);
						// mão
						anim[1].draw(pos_hand, 38, screen);
					#else
						SDL_RenderCopy(renderer, keyboard_bg, NULL, NULL);
						
						// tecla enter pressionada
						anim[0].draw(385, 80, renderer);
						// mão
						anim[1].draw(pos_hand, 38, renderer);
					#endif
					break;
					
				case KERNEL_HACKED:
					#ifndef USE_SDL2
						SDL_BlitSurface(kernel_site, NULL, screen, NULL);
					#else
						SDL_RenderCopy(renderer, kernel_site, NULL, NULL);
					#endif
					break;
					
				case LINUS_FACE:
					#ifndef USE_SDL2
						anim[3].draw(0,0,screen);
					#else
						anim[3].draw(0,0, renderer);
					#endif
					break;
					
				case LINUS_TALKING:
					
					break;
				
				case TUX_TALKING:
					
					break;
				
				case TUX_OUT:
					break;
				
				default:
					break;
			}
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case INACTIVE_INTRODUCTION:
					break;
				case PARALLEL_UNIVERSE:
					if (anim[5].update() == 3)
					{
						anim[4].reset();
						set_state(LINUS_TYPING);
					}
					break;
				case LINUS_TYPING:
					{
						int upd = anim[4].update();
						
						if (upd == 2)
						{
							CSoundPlayer::instance()->play_sound("key_press.wav");
						}
						else if (upd == 3)
						{
							pos_hand = init_hand;
							anim[0].reset();
							anim[1].pause();
							anim[1].reset();
							set_state(ENTER_KEYDOWN);
						}
					}
					break;
					
				case ENTER_KEYDOWN:
					pos_hand += vel_hand;
					if (pos_hand > final_hand)
					{
						pos_hand = final_hand;
						anim[1].play();
					}
					
					anim[1].update();
					if (anim[1].get_index() == 1)
					{
						int upd = anim[0].update();
						
						if (upd == 2 && anim[0].get_index() == 0)
						{
							CSoundPlayer::instance()->play_sound("key_press.wav");
						}
						
						if (upd == 3)
						{
							anim[2].reset();
							set_state(KERNEL_HACKED);
						}
					}
					break;
					
				case KERNEL_HACKED:
					if (anim[2].update() == 3)
					{
						anim[3].reset();
						set_state(LINUS_FACE);
					}
					break;
					
				case LINUS_FACE:
					if (anim[3].update() == 3)
					{
						set_state(LINUS_TALKING);
					}
					break;
					
				case LINUS_TALKING:
					set_state(TUX_TALKING);
					break;
				
				case TUX_TALKING:
					set_state(TUX_OUT);
					break;
					
				case TUX_OUT:
					set_state(INACTIVE_INTRODUCTION);
					break;
				
				default:
					set_state(LINUS_TYPING);
					break;
			}
		
			return get_state();
		}
};

#endif
