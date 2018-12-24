#include "gameover.hpp"

CGameOver::CGameOver ( SDL_Renderer * r )
{
	SDL_Surface * aux = 0;
	SDL_Texture * texture = 0;
	
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

	background = IMG_LoadTexture(r, path);
	
	if (!background)
	{
		printf("path = %s\n", path);
		printf("SDL_Error: %s\n", SDL_GetError());
		throw "SGameOver: não conseguiu abrir gameover_BG\n";
	}
	
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

	title = IMG_LoadTexture(r, path);

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

	texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "SGameOver: não conseguiu abrir gameover_stars\n";

	t_pos.x = (texture_width(background) - texture_width(title))/2.0f;
	stars.add_frame(texture, (SDL_Rect){0,0,296,184}, 30);
	stars.add_frame(texture, (SDL_Rect){0,184,296,184}, 30);
	stars.add_frame(texture, (SDL_Rect){0,184*2,296,184}, 30);
	stars.add_frame(texture, (SDL_Rect){0,184*3,296,184}, 30);
	stars.add_frame(texture, (SDL_Rect){0,184*4,296,184}, 30);
	stars.add_frame(texture, (SDL_Rect){0,184*5,296,184}, 30);
	stars.add_frame(texture, (SDL_Rect){0,184*6,296,184}, 30);
	s_pos = Vect(254,220);
}

CGameOver::~CGameOver (  )
{
	if (background)
		SDL_DestroyTexture(background);

	if (title)
		SDL_DestroyTexture(title);

	stars.destroy_textures();
}

void CGameOver::reset (  )
{
	stars.reset();
}

int CGameOver::update (  )
{
	stars.update();
	return get_state();
}

void CGameOver::draw ( Camera * cam, SDL_Renderer * renderer )
{
	SDL_Rect d;

	SDL_RenderCopy(renderer, background, NULL, NULL);

	d.x = t_pos.x;
	d.y = t_pos.y;
	d.w = texture_width(title);
	d.h = texture_height(title);
	SDL_RenderCopy(renderer, title, NULL, &d);

	stars.draw(renderer, s_pos.x, s_pos.y);
}


