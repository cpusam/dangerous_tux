#if _WIN32 || _WIN64 || __MINGW32__
	#include <windows.h>
	#include <direct.h>
	#include "SDL\\SDL.h"
	#include "SDL\\SDL_image.h"
	#include "SDL\\SDL_ttf.h"
	#undef main
#else
	#include <unistd.h>
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_ttf.h>
#endif

#include <iostream>
#include <exception>
#include <ctime>

#if _WIN32 || _WIN64 || __MINGW32__
	#include "light_engine\\light.hpp"
#else
	#include "light_engine/light.hpp"
#endif

#include "gamescreen.hpp" // cont√©m todos os headers do jogo
#include "gamevideo.hpp"


#define TILESIZE 48
#define FPS 50

bool control_fps ( int time_first )
{
	static int fps = 1000/FPS;
	int time_now = SDL_GetTicks() - time_first;

	if (time_now < fps)
	{
		SDL_Delay(fps - time_now);
		return true;
	}

	return false;
}

int main ( int argc, char **argv )
{
	try
	{
		SDL_putenv("SDL_VIDEO_CENTERED=center");

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw SDL_GetError();
	
		if (TTF_Init() < 0)
			throw "Erro na inicializaÁ„o da font\n";

		if (IMG_Init(IMG_INIT_PNG) == 0)
			throw "Erro na inicializaÁ„o da sdl image\n";
		
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
			throw SDL_GetError();
		
		SDL_Surface * screen;
		SDL_Event event;

		// no tamanho aproximado do dangerous dave original
		screen = set_screen(TILESIZE * 20, TILESIZE * 13);
		
		if (!screen)
			throw SDL_GetError();

		CPlayer player;
		
		CCamera cam((SDL_Rect){0,TILESIZE, TILESIZE * 20,TILESIZE * 10}, (SDL_Rect){0,0,0,0});
		
		cam.set_focus(SVect(screen->w/2, TILESIZE));

		CGameScreen gamescreen(screen, &cam, &player, TILESIZE);
		
		srand(time(0));
		
		int done = 0;
		Uint32 time_now;
		while (!done)
		{
			time_now = SDL_GetTicks();
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					done = 1;

				gamescreen.input(event);
			}
		
			if (control_fps(time_now))
			{
				gamescreen.update();
				gamescreen.draw();
				SDL_UpdateRect(screen, 0,0,0,0);
			}
		}
	}
	catch (const char * e)
	{
		cout << "Erro: " << e << endl;
		return 1;
	}
	// Erros com alguma lib SDL
	catch (char * e)
	{
		cout << "Erro: " << e << endl;
		return 1;
	}
	catch (exception & e)
	{
		cout << e.what() << endl;
		return 1;
	}
	
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();

	return 0;
}

