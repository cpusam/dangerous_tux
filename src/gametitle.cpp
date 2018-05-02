#include "gametitle.hpp"

CGameTitle::CGameTitle ( SDL_Renderer * r )
{
	SDL_Surface * aux = 0;
    SDL_Texture * texture = 0;
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
	
    background = IMG_LoadTexture(r, path);

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

	title_name = IMG_LoadTexture(r, path);

	if (!title_name)
		throw "CGameTitle: não conseguiu carregar imagem de título\n";


	#if _WIN32 || _WIN64
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
	
		texture = IMG_LoadTexture(r, path);
		if (!texture)
			throw "CGameTitle: não conseguiu carregar imagem do tux no foguete\n";
		//um bug  aqui, dois bugs aqui, 3 antes de 300 era 3 quadros e eu mudei para 300 milissegundos
		tux_rocket.add_frame(texture, (SDL_Rect){0,0,319,624}, 300);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599,319,624}, 300);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599*2,319,624}, 300);
		
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
		
		tux_rocket.add_frame(texture, (SDL_Rect){0,0,319,624}, 300);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599,319,624}, 300);
		tux_rocket.add_frame(texture, (SDL_Rect){0,599*2,319,624}, 300);

	tn_final = Vect(34,60);
	tn_init = tn_pos = Vect(-texture_width(title_name), tn_final.y);

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
	if (!Writer::instance()->load_font(path, path, 70))
		throw "CGameTitle: não foi possível carregar fonte\n";

	press_enter = new GuiLabel("PRESS ENTER!", (SDL_Color){0,0,0,0});
	press_enter->set_pos(Vect(188,439));
	version = new GuiLabel("ALPHA VERSION - 2018", (SDL_Color){0,255,0,255});

	version->set_pos(Vect((960 - version->get_texture_width())/2, 624 - version->get_texture_height()));
	texture = press_enter->get_texture();
	p_enter.add_frame(texture, (SDL_Rect){0,0,texture_width(texture),texture_height(texture)}, 400);
	p_enter.add_frame(0, (SDL_Rect){0,0,0,0}, 400);
}

CGameTitle::~CGameTitle (  )
{
	if (background)
		SDL_DestroyTexture(background);

	if (title_name)
		SDL_DestroyTexture(title_name);

	tux_rocket.destroy_textures();

	delete press_enter;
	delete version;
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

			tux_rocket.draw(renderer, tr_pos.x, tr_pos.y);
			p_enter.draw(renderer, press_enter->get_pos().x, press_enter->get_pos().y);
			version->draw(renderer);
			break;
			
		case BACKGROUND_GAMETITLE:
			SDL_RenderCopy(renderer, background, NULL, NULL);

			tux_rocket.draw(renderer, tr_pos.x, tr_pos.y);
			version->draw(renderer);
			break;

		default:
			break;
	}
}




