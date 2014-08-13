#include "gametransition.hpp"

#ifndef USE_SDL2
	CGameTransition::CGameTransition (  )
#else
	CGameTransition::CGameTransition ( SDL_Renderer * r )
#endif
{
	cam = 0;
	phrase = 0;
	map = new CTileMapView(48);
	
	#if _WIN32 || _WIN64
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
	
	#ifndef USE_SDL2
		map->surface = optimize_surface_alpha(IMG_Load(path));
		if (!map->surface)
			throw "CGameTransition: não foi possível carregar tiles.png\n";
	#else
		renderer = r;
		map->texture = IMG_LoadTexture(renderer, path);
		if (!map->texture)
			throw "CGameTransition: não foi possível carregar tiles.png\n";
	#endif
}

CGameTransition::~CGameTransition (  )
{
	delete map;

	if (phrase)
		delete phrase;
}

#ifndef USE_SDL2
	void CGameTransition::set_cam ( CCamera * c, SDL_Surface * screen )
#else
	void CGameTransition::set_cam ( CCamera * c, SDL_Renderer * renderer )
#endif
{
	if (cam)
		delete cam;

	SDL_Rect d = c->get_dimension();
	#ifndef USE_SDL2
		cam = new CCamera((SDL_Rect){0, (screen->h - d.h)/2, d.w,d.h}, (SDL_Rect){0,0,d.w,d.h});
	#else
		int h;
		SDL_RenderGetLogicalSize(renderer, NULL, &h);
		cam = new CCamera((SDL_Rect){0, (h - d.h)/2, d.w,d.h}, (SDL_Rect){0,0,d.w,d.h});
	#endif
}

void CGameTransition::set_bg ( string path )
{
	#ifndef USE_SDL2
		bg.set_surface(optimize_surface_alpha(IMG_Load(path.c_str())));
		if (!bg.get_surface())
			throw "CGameTransition: não foi possível carregar background\n";
	#else
		SDL_Surface * aux = IMG_Load(path.c_str());
		bg.set_texture(SDL_CreateTextureFromSurface(renderer, aux));
		SDL_FreeSurface(aux);
		if (!bg.get_texture())
			throw "CGameTransition: não foi possível carregar background\n";
	#endif
}

void CGameTransition::set_player ( CPlayer * p )
{
	player = p;
}

void CGameTransition::reset ( int curr_level, int num_levels )
{
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
	#else
		char path[1024];
	#endif
	if (rand() % 2)
	{
		#if _WIN32 || _WIN64
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
		#if _WIN32 || _WIN64
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
	for (i = 0, tile = -1; (tile = map->get_tile(i)) != -1; i++)
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
	if (!CWriter::instance()->set_font(path, 80))
		throw "CGameTransition: não conseguiu carregar font\n";
	
	#if USE_SDL2
		CWriter::instance()->set_renderer(renderer);
	#endif
	
	if (num_levels - curr_level - 1 > 1)
		sprintf(path, "GOOD WORK! ONLY %d LEVELS TO GO", num_levels - curr_level - 1);
	else
		sprintf(path, "THIS IS THE LAST LEVEL!!!");
	
	if (phrase)
		delete phrase;

	phrase = new CLabel(path, (SDL_Color){255,255,0,0});
	#ifndef USE_SDL2
		phrase->set_pos(SVect((960 - phrase->get_surface()->w)/2, 0));
	#else
		phrase->set_pos(SVect((960 - phrase->get_texture_width())/2, 0));
	#endif
}

#ifndef USE_SDL2
	void CGameTransition::draw ( SDL_Surface * screen )
	{
		bg.draw(cam, screen);
		map->draw(cam, screen);
		player->draw(cam, screen);
		phrase->draw(screen);
	}
#else
	void CGameTransition::draw (  )
	{
		SDL_SetRenderDrawColor(renderer, 0,0,0,0xFF);
		SDL_RenderClear(renderer);
		bg.draw(cam, renderer);
		map->draw(cam, renderer);
		player->draw(cam, renderer);
		phrase->draw(renderer);
	}
#endif

int CGameTransition::update (  )
{
	if (get_state() == 1)
	{
		if (player->update() == INACTIVE_PLAYER)
			set_state(0);
	}
	
	return get_state();
}


