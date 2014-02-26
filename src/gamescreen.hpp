#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include "gameevent_manager.hpp"
#include "player.hpp"
#include "level.hpp"
#include "highscore.hpp"
#include "savegame.hpp"

struct SGameOver
{
	SDL_Surface * background;
	SDL_Surface * title;
	CAnimation stars;
	
	SVect t_pos;
	SVect s_pos;
	
	SGameOver (  )
	{
		background = 0;
		title = 0;
		stars.surface = 0;
	}
	
	~SGameOver (  )
	{
		if (background)
			SDL_FreeSurface(background);
		
		if (title)
			SDL_FreeSurface(title);
		
		if (stars.surface)
			SDL_FreeSurface(stars.surface);
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
	
	void reset (  )
	{
		stars.reset();
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

enum EGameIntroductionState
{
	PARALLEL_UNIVERSE,
	LINUS_TYPING,
	ENTER_KEYDOWN,
	KERNEL_HACKED,
	LINUS_FACE,
	LINUS_TUX_TALKING,
	TUX_OUT,
	INACTIVE_INTRODUCTION
};

class CGameIntroduction: public CStateMachine
{
	protected:
		vector <CAnimation> anim;
		SDL_Surface * browser_bg;
		SDL_Surface * keyboard_bg;
		SDL_Surface * kernel_site;
		CLabel * parallel; // frase: "In a parallel universe..."
		int pos_hand; // posição da mão
		int init_hand; // posição inicial da mão
		int final_hand; // posição final da mão
		int vel_hand; // velocidade de movimento da mão
	
	public:
		CGameIntroduction (  )
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
			
			browser_bg = optimize_surface_alpha(IMG_Load(path));
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
			
			keyboard_bg = optimize_surface_alpha(IMG_Load(path));
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
			
			kernel_site = optimize_surface_alpha(IMG_Load(path));
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

			parallel = new CLabel("In a parallel universe...", (SDL_Color){255,255,0,0});
			parallel->set_pos(SVect((960 - parallel->get_surface()->w)/2, (624 - parallel->get_surface()->h)/2));
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
			
			anim.resize(6);
			
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
			
			anim[0].surface = optimize_surface_alpha(IMG_Load(path));
			if (!anim[0].surface)
				throw "CGameIntroduction: não foi possível carregar scene2-Enter.png\n";
			
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
			
			anim[1].surface = optimize_surface_alpha(IMG_Load(path));
			if (!anim[1].surface)
				throw "CGameIntroduction: não foi possível carregar scene2-Hand.png\n";
			
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
			
			anim[4].surface = optimize_surface_alpha(IMG_Load(path));
			if (!anim[4].surface)
				throw "CGameIntroduction: não foi possível carregar scene1-Kernel.png\n";
			
			anim[0].set_repeat(false); // animação da tecla enter pressionada
			anim[0].add_frame((SDL_Rect){0,0,270,269}, 1);
			anim[0].add_frame((SDL_Rect){0,269,270,269}, 6);
			anim[0].add_frame((SDL_Rect){0,269*2,270,269}, 20);

			anim[1].set_repeat(false); // animação da mão
			anim[1].pause();
			anim[1].add_frame((SDL_Rect){0,0,698,585}, 15);
			anim[1].add_frame((SDL_Rect){0,585,698,585}, 15);
			
			anim[2].add_frame((SDL_Rect){0,0,0,0}, 75); // animação do hack do kernel
			anim[2].set_repeat(false);

			anim[3].add_frame((SDL_Rect){0,0,0,0}, 15); // animação do rosto de Linus
			anim[3].add_frame((SDL_Rect){0,0,0,0}, 15);
			anim[3].set_repeat(false);
			
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
			
			init_hand = -anim[1].surface->w/2;
			final_hand = -(anim[1].surface->w - 567); 
			pos_hand = init_hand;
			vel_hand = 20;
			reset();
		}
		
		~CGameIntroduction (  )
		{
			if (parallel)
				delete parallel;

			if (browser_bg)
				SDL_FreeSurface(browser_bg);
			
			if (keyboard_bg)
				SDL_FreeSurface(keyboard_bg);
			
			for (int i(0); i < anim.size(); i++)
				if (anim[i].surface)
					SDL_FreeSurface(anim[i].surface);
		}
		
		void reset (  )
		{
			anim[5].reset();
			set_state(PARALLEL_UNIVERSE);
		}
		
		void draw ( SDL_Surface * screen )
		{
			switch (get_state())
			{
				case PARALLEL_UNIVERSE:
					SDL_FillRect(screen, NULL, 0x0);
					parallel->draw(screen);
					break;
				case LINUS_TYPING:
					SDL_BlitSurface(browser_bg, NULL, screen, NULL);
					if (anim[4].surface)
						anim[4].draw(206,206, screen);
					break;
					
				case ENTER_KEYDOWN:
					SDL_BlitSurface(keyboard_bg, NULL, screen, NULL);

					// tecla enter pressionada
					if (anim[0].surface)
						anim[0].draw(385, 80, screen);
					
					// mão
					if (anim[1].surface)
						anim[1].draw(pos_hand, 38, screen);
					break;
					
				case KERNEL_HACKED:
					SDL_BlitSurface(kernel_site, NULL, screen, NULL);
					break;
					
				case LINUS_FACE:
					break;
					
				case LINUS_TUX_TALKING:
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
					if (anim[4].update() == 3)
					{
						pos_hand = init_hand;
						anim[0].reset();
						anim[1].pause();
						anim[1].reset();
						set_state(ENTER_KEYDOWN);
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
						if (anim[0].update() == 3)
						{
							anim[2].reset();
							set_state(KERNEL_HACKED);
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
						set_state(LINUS_TUX_TALKING);
					}
					break;
					
				case LINUS_TUX_TALKING:
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

class CGameTransition: public CStateMachine
{
	protected:
		CBackground bg;
		CCamera * cam;
		CTileMapView * map;
		CPlayer * player;
	
	public:
		CGameTransition (  )
		{
			cam = 0;
			map = new CTileMapView(48);
			
			#if _WIN32 || _WIN64 || __MINGW32__
				char path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
				#ifndef PREFIX
					sprintf(path, "%s\\images\\tiles.png", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\images\\tiles.png", PREFIX);
				#endif
			#else
				char path[1024];
				#ifndef PREFIX
					sprintf(path, "./images/tiles.png");
				#else
					sprintf(path, "%s/share/games/dangeroustux/images/tiles.png", PREFIX);
				#endif
			#endif
			map->surface = optimize_surface_alpha(IMG_Load(path));
			if (!map->surface)
				throw "CGameTransition: não foi possível carregar tiles.png\n";
		}
		
		~CGameTransition (  )
		{
			delete map;
		}
		
		void set_cam ( CCamera * c, SDL_Surface * screen )
		{
			if (cam)
				delete cam;

			SDL_Rect d = c->get_dimension();
			cam = new CCamera((SDL_Rect){0, (screen->h - d.h)/2, d.w,d.h}, (SDL_Rect){0,0,d.w,d.h});
		}
		
		void set_bg ( string path )
		{
			bg.surface = optimize_surface_alpha(IMG_Load(path.c_str()));
			if (!bg.surface)
				throw "CGameTransition: não foi possível carregar background\n";
		}
		
		void set_player ( CPlayer * p )
		{
			player = p;
		}
		
		void reset (  )
		{
			#if _WIN32 || _WIN64 || __MINGW32__
				char path[FILENAME_MAX];
				char p2[FILENAME_MAX];
				_getcwd(p2, sizeof(p2));
			#else
				char path[1024];
			#endif
			if (rand() % 2)
			{
				#if _WIN32 || _WIN64 || __MINGW32__
					#ifndef PREFIX
						sprintf(path, "%s\\levels\\transition1.txt", p2);
					#else
						sprintf(path, "%s\\dangeroustux\\levels\\transition1.txt", PREFIX);
					#endif
				#else
					#ifndef PREFIX
						sprintf(path, "./levels/transition1.txt");
					#else
						sprintf(path, "%s/share/games/dangeroustux/levels/transition1.txt", PREFIX);
					#endif
				#endif
			}
			else
			{
				#if _WIN32 || _WIN64 || __MINGW32__
					#ifndef PREFIX
						sprintf(path, "%s\\levels\\transition2.txt", p2);
					#else
						sprintf(path, "%s\\dangeroustux\\levels\\transition2.txt", PREFIX);
					#endif
				#else
					#ifndef PREFIX
						sprintf(path, "./levels/transition2.txt");
					#else
						sprintf(path, "%s/share/games/dangeroustux/levels/transition2.txt", PREFIX);
					#endif
				#endif
			}
			
			if (!map->read(path))
				throw "CGameTransition: não foi possível carregar o mapa\n";
			
			int ts = map->get_tilesize();
			map->remove_tile('.'); // remove o tile vazio da visão
			map->remove_tile('P'); // remove o jogador da visão
			map->set_source('a', (SDL_Rect){0,0,ts,ts});
			map->set_source('b', (SDL_Rect){ts,0,ts,ts});
			map->set_source('c', (SDL_Rect){ts*2,0,ts,ts});
			map->set_source('E', (SDL_Rect){ts*7,0,ts,ts});
			map->set_source('i', (SDL_Rect){0,ts,ts,ts});
			map->set_source('j', (SDL_Rect){ts,ts,ts,ts});
			map->set_source('k', (SDL_Rect){ts*2,ts,ts,ts});
			map->set_source('l', (SDL_Rect){ts*3,ts,ts,ts});
			map->set_source('m', (SDL_Rect){ts*4,ts,ts,ts});
			map->set_source('n', (SDL_Rect){ts*5,ts,ts,ts});
			map->set_source('s', (SDL_Rect){ts*6,ts*2,ts,ts});
			map->set_source('t', (SDL_Rect){ts*7,ts*2,ts,ts});
			
			cam->set_limit(map->get_dimension());
			
			float final_pos = cam->get_dimension().w - 48 * 2;
			SVect p;
			
			int i, tile;
			for (i = 0, tile; (tile = map->get_tile(i)) != -1; i++)
				if (tile == 'P')
				{
					p.x = (i % map->get_width()) * map->get_tilesize();
					p.y = (i / map->get_width()) * map->get_tilesize();
					break;
				}
			
			if (i >= map->get_width() * map->get_height())
				throw "CGameTransition: cadê o 'P' no mapa de transição?\n";
			
			player->set_transition(map, p, final_pos);
			set_state(1);
		}
		
		void draw ( SDL_Surface * screen )
		{
			bg.draw(cam, screen);
			map->draw(cam, screen);
			player->draw(cam, screen);
		}
		
		int update (  )
		{
			if (get_state() == 1)
			{
				if (player->update() == INACTIVE_PLAYER)
					set_state(0);
			}
			
			return get_state();
		}
};

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
		int curr_level; // level atual
		vector <CLevel *> levels; // os levels
		SDL_Surface * screen;
		CCamera * cam;
		CPlayer * player;
		int any_key, enter_key, pause_key;
		int tilesize;
		
		SGameOver gameover;
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
			tilesize = ts;
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
							transition.reset();
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
						set_state(HIGHSCORE_SCREEN); // vai para tela de high score
					}
					
					gameover.stars.update();
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

