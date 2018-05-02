#include "gameintroduction.hpp"

CGameIntroduction::CGameIntroduction ( SDL_Renderer * r )
{
	SDL_Surface * aux = 0;
	SDL_Texture * texture = 0;

	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		char paux[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
	#else
		char path[1024];
		char paux[1024];
	#endif
	
	#if _WIN32 || _WIN64
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

	browser_bg = IMG_LoadTexture(r, path);
	if (!browser_bg)
		throw "CGameIntroduction: não foi possível carregar scene1_BG.png\n";
	
	#if _WIN32 || _WIN64
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

	keyboard_bg = IMG_LoadTexture(r, path);
	if (!keyboard_bg)
		throw "CGameIntroduction: não foi possível carregar scene2_BG.png\n";
	
	#if _WIN32 || _WIN64
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

	kernel_site = IMG_LoadTexture(r, path);
	if (!kernel_site)
		throw "CGameIntroduction: não foi possível carregar scene3_BG.png\n";
	
	#if _WIN32 || _WIN64
		#ifndef PREFIX
			sprintf(path, "%s\\images\\intro_scene\\scene5.jpg", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\images\\intro_scene\\scene5.jpg", PREFIX);
		#endif
	#else
		#ifndef PREFIX
			sprintf(path, "./images/intro_scene/scene5.jpg");
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/intro_scene/scene5.jpg", PREFIX);
		#endif
	#endif

	igloo_bg = IMG_LoadTexture(r, path);
	if (!igloo_bg)
		throw "CGameIntroduction: não foi possível carregar scene5.jpg\n";
	
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

	if (!Writer::instance()->load_font(path, path, 80))
		throw "CGameIntroduction: não foi possível carregar font\n";
	
	Writer::instance()->set_renderer(r);

	parallel = new GuiLabel("IN A PARALLEL UNIVERSE...", (SDL_Color){255,255,0,255});
	later = new GuiLabel("SOME TIME LATER...", (SDL_Color){255,255,0,255});
	parallel->set_pos(Vect((960 - parallel->get_texture_width())/2, (624 - parallel->get_texture_height())/2));
	later->set_pos(Vect((960 - later->get_texture_width())/2, (624 - later->get_texture_height())/2));
	/*
	#if _WIN32 || _WIN64
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
	
	SoundFX sound;
	#if _WIN32 || _WIN64
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
	SoundPlayer::instance()->add_sound(sound);
	
	anim.resize(7 + 5);
	
	#if _WIN32 || _WIN64
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

	texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CGameIntroduction: não foi possível carregar scene2-Enter.png\n";

	anim[0].set_repeat(false); // animação da tecla enter pressionada
	anim[0].add_frame(texture, (SDL_Rect){0,0,270,269}, 1);
	anim[0].add_frame(texture, (SDL_Rect){0,269,270,269}, 2);
	anim[0].add_frame(texture, (SDL_Rect){0,269*2,270,269}, 20);

	#if _WIN32 || _WIN64
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

	texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CGameIntroduction: não foi possível carregar scene2-Hand.png\n";

	anim[1].set_repeat(false); // animação da mão
	anim[1].pause();
	anim[1].add_frame(texture, (SDL_Rect){0,0,698,585}, 15);
	anim[1].add_frame(texture, (SDL_Rect){0,585,698,585}, 15);

	#if _WIN32 || _WIN64
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

	texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CGameIntroduction: não foi possível carregar scene4.png\n";

	anim[3].set_repeat(false); // animação do rosto de Linus
	anim[3].add_frame(texture, (SDL_Rect){0,0,960,624}, 20);
	anim[3].add_frame(texture, (SDL_Rect){0,624,960,624}, 75);

	#if _WIN32 || _WIN64
		#ifndef PREFIX
			sprintf(paux, "%s\\images\\intro_scene\\", p2);
		#else
			sprintf(paux, "%s\\dangeroustux\\images\\intro_scene\\", PREFIX);
		#endif
	#else
		#ifndef PREFIX
			sprintf(paux, "./images/intro_scene/");
		#else
			sprintf(paux, "%s/share/games/dangeroustux/images/intro_scene/", PREFIX);
		#endif
	#endif
	
	#if _WIN32 || _WIN64
		sprintf(path, "%s\\scene1-Kernel.png", paux);
	#else
		sprintf(path, "%s/scene1-Kernel.png", paux);
	#endif

	#if _WIN32 || _WIN64
		sprintf(path, "%s\\scene1-Kernel.png", paux);
	#else
		sprintf(path, "%s/scene1-Kernel.png", paux);
	#endif

	texture = IMG_LoadTexture(r, path);
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

	/* EM CONSTRUÇÃO
	#if _WIN32 || _WIN64
		sprintf(path, "%s\\Linus\\Linus-Boca-falando_sorrindo.png", paux);
	#else
		sprintf(path, "%s/Linus/Linus-Boca-falando_sorrindo.png", paux);
	#endif
	aux = IMG_Load(path);
	texture = SDL_CreateTextureFromSurface(r, aux);
	if (!texture)
		throw "CGameIntroduction: não foi possível carregar Linus/Linus-Boca-falando_sorrindo.png";

	anim[8].add_frame(texture, (SDL_Rect){}, 15); // animação boca falando
	anim[8].add_frame(texture, (SDL_Rect){}, 15);
	anim[8].add_frame(texture, (SDL_Rect){}, 15);
	anim[8].add_frame(texture, (SDL_Rect){}, 15);
	anim[8].set_repeat(true);

	anim[9].add_frame(texture, (SDL_Rect){}, 15); // animação boca sorrindo
	anim[9].add_frame(texture, (SDL_Rect){}, 15);
	anim[9].add_frame(texture, (SDL_Rect){}, 15);
	anim[9].add_frame(texture, (SDL_Rect){}, 15);
	anim[9].add_frame(texture, (SDL_Rect){}, 15);
	anim[9].set_repeat(false);
	*/

	// animação do hack do kernel
	anim[2].add_frame(0, (SDL_Rect){0,0,0,0}, 75);
	anim[2].set_repeat(false);
	 // em um universo paralelo
	anim[5].add_frame(0, (SDL_Rect){0,0,0,0}, 100);
	anim[5].set_repeat(false);
	// algum tempo depois
	anim[6].add_frame(0, (SDL_Rect){0,0,0,0}, 60);
	anim[6].set_repeat(false);
	// mostrando lado de fora do iglu
	anim[7].add_frame(0, (SDL_Rect){0,0,0,0}, 60);
	anim[7].set_repeat(false);

	init_hand = -texture_width(anim[1].get_texture(0))/2;
	final_hand = -(texture_width(anim[1].get_texture(0)) - 567);
	pos_hand = init_hand;
	vel_hand = 20;
	reset();
}

CGameIntroduction::~CGameIntroduction (  )
{
	if (parallel)
		delete parallel;
		
	if (later)
		delete later;

	if (browser_bg)
		SDL_DestroyTexture(browser_bg);

	if (keyboard_bg)
		SDL_DestroyTexture(keyboard_bg);

	if (kernel_site)
		SDL_DestroyTexture(kernel_site);

	if (igloo_bg)
		SDL_DestroyTexture(igloo_bg);

	for (int i(0); i < anim.size(); i++)
		anim[i].destroy_textures();
}

void CGameIntroduction::reset (  )
{
	for (int i(0); i < anim.size(); i++)
		anim[i].reset();

	set_state(PARALLEL_UNIVERSE);
}

void CGameIntroduction::draw ( SDL_Renderer * renderer )
{
	switch (get_state())
	{
		case PARALLEL_UNIVERSE:
			SDL_SetRenderDrawColor(renderer, 0,0,0,255);
			SDL_RenderFillRect(renderer, NULL);
			parallel->draw(renderer);
			break;
		case LINUS_TYPING:
			SDL_RenderCopy(renderer, browser_bg, NULL, NULL);
			anim[4].draw(renderer, 206,206);
			break;
			
		case ENTER_KEYDOWN:
			SDL_RenderCopy(renderer, keyboard_bg, NULL, NULL);

			// tecla enter pressionada
			anim[0].draw(renderer, 385, 80);
			// mão
			anim[1].draw(renderer, pos_hand, 38);
			break;
			
		case HACKED_KERNEL:
			SDL_RenderCopy(renderer, kernel_site, NULL, NULL);
			break;
			
		case LINUS_FACE:
			anim[3].draw(renderer, 0,0);
			break;
		
		case SOMETIME_LATER:
			SDL_SetRenderDrawColor(renderer, 0,0,0,255);
			SDL_RenderFillRect(renderer, NULL);
			later->draw(renderer);
			break;
		
		case IGLOO_BACKGROUND:
			SDL_RenderCopy(renderer, igloo_bg, NULL, NULL);
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

int CGameIntroduction::update (  )
{
	switch (get_state())
	{
		case INACTIVE_INTRODUCTION:
			break;
		case PARALLEL_UNIVERSE:
			if (anim[5].update() == 4)
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
					SoundPlayer::instance()->play_sound("key_press.wav");
				}
				else if (upd == 4)
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
					SoundPlayer::instance()->play_sound("key_press.wav");
				}
				
				if (upd == 4)
				{
					anim[2].reset();
					set_state(HACKED_KERNEL);
				}
			}
			break;
			
		case HACKED_KERNEL:
			if (anim[2].update() == 4)
			{
				anim[3].reset();
				set_state(LINUS_FACE);
			}
			break;
		
		case LINUS_FACE:
			if (anim[3].update() == 4)
			{
				set_state(SOMETIME_LATER);
			}
			break;
		
		case SOMETIME_LATER:
			if (anim[6].update() == 4)
			{
				set_state(IGLOO_BACKGROUND);
			}
			break;
		
		case IGLOO_BACKGROUND:
			if (anim[7].update() == 4)
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


