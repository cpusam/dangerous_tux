#include "gametitle.hpp"

#ifndef USE_SDL2
	CGameTitle::CGameTitle (  )
#else
	CGameTitle::CGameTitle ( SDL_Renderer * r )
#endif
{
	SDL_Surface * aux = 0;
	#if USE_SDL2
		SDL_Texture * texture = 0;
	#endif
	press_enter = 0;
	version = 0;
	
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
	#else
		char path[1024];
	#endif

	#if _WIN32 || _WIN64
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
	
	#ifndef USE_SDL2
		background = optimize_surface_alpha(IMG_Load(path));
	#else
		background = IMG_LoadTexture(r, path);
	#endif
		
	if (!background)
		throw "CGameTitle: não conseguiu carregar imagem de background\n";

	
	#if _WIN32 || _WIN64
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

	#ifndef USE_SDL2
		title_name = optimize_surface_alpha(IMG_Load(path));
	#else
		title_name = IMG_LoadTexture(r, path);
	#endif

	if (!title_name)
		throw "CGameTitle: não conseguiu carregar imagem de título\n";


	#if _WIN32 || _WIN64
		#ifndef PREFIX
			#ifndef USE_SDL2
				sprintf(path, "%s\\images\\tux_rocket_sprites_full.png", p2);
			#else
				sprintf(path, "%s\\images\\tux_rocket_sprites.png", p2);
			#endif
		#else
			#ifndef USE_SDL2
				sprintf(path, "%s\\dangeroustux\\images\\tux_rocket_sprites_full.png", PREFIX);
			#else
				sprintf(path, "%s\\dangeroustux\\images\\tux_rocket_sprites.png", PREFIX);
			#endif
		#endif
	#else
		#ifndef PREFIX
			#ifndef USE_SDL2
				sprintf(path, "./images/tux_rocket_sprites_full.png");
			#else
				sprintf(path, "./images/tux_rocket_sprites.png");
			#endif
		#else
			#ifndef USE_SDL2
				sprintf(path, "%s/share/games/dangeroustux/images/tux_rocket_sprites_full.png", PREFIX);
			#else
				sprintf(path, "%s/share/games/dangeroustux/images/tux_rocket_sprites.png", PREFIX);
			#endif
		#endif
	#endif
	
	#ifndef USE_SDL2
		aux = optimize_surface_alpha(IMG_Load(path));
		if (!aux)
			throw "CGameTitle: não conseguiu carregar imagem do tux no foguete\n";
		
		tux_rocket.add_frame(aux, (SDL_Rect){0,0,319,599}, 3);
		tux_rocket.add_frame(aux, (SDL_Rect){0,599,319,599}, 3);
		tux_rocket.add_frame(aux, (SDL_Rect){0,599*2,319,599}, 3);
		tux_rocket.add_frame(aux, (SDL_Rect){0,599*3,319,599}, 3);
		tux_rocket.add_frame(aux, (SDL_Rect){0,599*4,319,599}, 3);
		tux_rocket.add_frame(aux, (SDL_Rect){0,599*5,319,599}, 3);
	#else
		texture = IMG_LoadTexture(r, path);
		if (!texture)
			throw "CGameTitle: não conseguiu carregar imagem do tux no foguete\n";
		
		tux_rocket.add_frame(texture, (SDL_Rect){0,0,319,599}, 3);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599,319,599}, 3);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599*2,319,599}, 3);
		
		#if _WIN32 || _WIN64
			#ifndef PREFIX
				sprintf(path, "%s\\images\\tux_rocket_sprites2.png", p2);
			#else
				sprintf(path, "%s\\dangeroustux\\images\\tux_rocket_sprites2.png", PREFIX);
			#endif
		#else
			#ifndef PREFIX
				sprintf(path, "./images/tux_rocket_sprites2.png");
			#else
				sprintf(path, "%s/share/games/dangeroustux/images/tux_rocket_sprites2.png", PREFIX);
			#endif
		#endif
		
		texture = IMG_LoadTexture(r, path);
		if (!texture)
			throw "CGameTitle: não conseguiu carregar segunda imagem do tux no foguete\n";
		
		tux_rocket.add_frame(texture, (SDL_Rect){0,0,319,599}, 3);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599,319,599}, 3);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599*2,319,599}, 3);
	#endif

	tn_final = SVect(34,60);
	#ifndef USE_SDL2
		tn_init = tn_pos = SVect(-title_name->w, tn_final.y);
	#else
		tn_init = tn_pos = SVect(-texture_width(title_name), tn_final.y);
	#endif
	tn_vel = 10;
	tr_pos.x = 600;
	
	
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
	if (!CWriter::instance()->set_font(path, 70))
		throw "CGameTitle: não foi possível carregar fonte\n";

	press_enter = new CLabel("PRESS ENTER!", (SDL_Color){0,0,0,0});
	press_enter->set_pos(SVect(188,439));
	version = new CLabel("BETA VERSION - 2014", (SDL_Color){0,255,0,255});
	#ifndef USE_SDL2
		version->set_pos(SVect((960 - version->get_surface()->w)/2, 624 - version->get_surface()->h));
		p_enter.add_frame(NULL, (SDL_Rect){0,0,0,0}, 25);
		p_enter.add_frame(NULL, (SDL_Rect){0,0,0,0}, 25);

	#else
		version->set_pos(SVect((960 - version->get_texture_width())/2, 624 - version->get_texture_height()));
		p_enter.add_frame(0, (SDL_Rect){0,0,0,0}, 25);
		p_enter.add_frame(0, (SDL_Rect){0,0,0,0}, 25);
	#endif
}

CGameTitle::~CGameTitle (  )
{
	#ifndef USE_SDL2
		if (background)
			SDL_FreeSurface(background);

		if (title_name)
			SDL_FreeSurface(title_name);

		tux_rocket.destroy_surfaces();
	#else
		if (background)
			SDL_DestroyTexture(background);

		if (title_name)
			SDL_DestroyTexture(title_name);

		tux_rocket.destroy_textures();
	#endif
}

void CGameTitle::reset (  )
{
	tn_pos = tn_init;
	tux_rocket.reset();
	set_state(RUNNING_GAMETITLE);
}

void CGameTitle::input ( SDL_Event & event )
{
	if (event.type == SDL_KEYDOWN)
		if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
		{
			press_enter->show(false);
			set_state(BACKGROUND_GAMETITLE);
		}
}

int CGameTitle::update (  )
{
	switch (get_state())
	{
		case RUNNING_GAMETITLE:
			tn_pos.x += tn_vel;
			if (tn_pos.x > tn_final.x)
				tn_pos = tn_final;
	
			tux_rocket.update();
	
			p_enter.update();
			if (p_enter.get_index() == 1)
				press_enter->show(false);
			else
				press_enter->show();
			break;
		
		case BACKGROUND_GAMETITLE:
			tux_rocket.update();
			break;
		
		default:
			set_state(RUNNING_GAMETITLE);
			break;
	}
	
	return get_state();
}

#ifndef USE_SDL2
	void CGameTitle::draw ( SDL_Surface * screen )
	{
		switch (get_state())
		{
			case RUNNING_GAMETITLE:
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
				break;
			
			case BACKGROUND_GAMETITLE:
				SDL_BlitSurface(background, NULL, screen, NULL);

				tux_rocket.draw(tr_pos.x, tr_pos.y, screen);
				version->draw(screen);
				break;
			
			default:
				break;
		}
	}
#else
	void CGameTitle::draw ( SDL_Renderer * renderer )
	{
		switch (get_state())
		{
			case RUNNING_GAMETITLE:
				SDL_RenderCopy(renderer, background, NULL, NULL);

				SDL_Rect d;

				d.x = tn_pos.x;
				d.y = tn_pos.y;
				d.w = texture_width(title_name);
				d.h = texture_height(title_name);

				SDL_RenderCopy(renderer, title_name, NULL, &d);

				tux_rocket.draw(tr_pos.x, tr_pos.y, renderer);
				press_enter->draw(renderer);
				version->draw(renderer);
				break;
			
			case BACKGROUND_GAMETITLE:
				SDL_RenderCopy(renderer, background, NULL, NULL);

				tux_rocket.draw(tr_pos.x, tr_pos.y, renderer);
				version->draw(renderer);
				break;
			
			default:
				break;
		}
	}
#endif



