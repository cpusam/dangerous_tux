#if _WIN32 || _WIN64 || __MINGW32__
	#ifndef USE_SDL2
		#include "SDL\\SDL.h"
		#include "SDL\\SDL_image.h"
		#include "SDL\\SDL_ttf.h"
	#else
		#include "SDL2\\SDL.h"
		#include "SDL2\\SDL_image.h"
		#include "SDL2\\SDL_ttf.h"
	#endif

	#include <windows.h>
	#include <direct.h>
	#undef main
#else
	#include <unistd.h>
	#ifndef USE_SDL2
		#include <SDL/SDL.h>
		#include <SDL/SDL_image.h>
		#include <SDL/SDL_ttf.h>
	#else
		#include <SDL2/SDL.h>
		#include <SDL2/SDL_image.h>
		#include <SDL2/SDL_ttf.h>
	#endif
#endif

#include <iostream>
#include <exception>
#include <ctime>

#if _WIN32 || _WIN64 || __MINGW32__
	#include "light_engine\\light.hpp"
#else
	#include "light_engine/light.hpp"
#endif

#include "gamescreen.hpp" // contém todos os headers do jogo
#ifndef USE_SDL2
	#include "gamevideo.hpp"
#endif


#define TILESIZE 48

bool control_fps ( int time_first, int fps )
{
	fps = 1000/fps;
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
		#ifndef USE_SDL2
			SDL_putenv("SDL_VIDEO_CENTERED=center");
		#else
			#if _WIN32 || _WIN64 || __MINGW32__
				SDL_SetMainReady();
			#endif
		#endif

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0)
			throw SDL_GetError();
	
		if (TTF_Init() < 0)
			throw "Erro na inicialização da font\n";

		if (IMG_Init(IMG_INIT_PNG) == 0)
			throw "Erro na inicialização da sdl image\n";
		
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
			throw SDL_GetError();
		
		Mix_AllocateChannels(20); // aloca 20 canais para tocar sons

		SDL_Event event;

		int fps;
		int fullscreen = 0;
		#ifndef USE_SDL2
			// no tamanho aproximado do dangerous dave original
			SDL_Surface * screen = set_screen(TILESIZE * 20, TILESIZE * 13, &fps);
			if (!screen)
				throw SDL_GetError();
		#else
			SDL_Window * window = SDL_CreateWindow("Dangerous Tux! BETA version", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TILESIZE * 20, TILESIZE * 13, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
			SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // em fase de testes
			SDL_RenderSetLogicalSize(renderer, TILESIZE * 20, TILESIZE * 13);
			fps = 40;
		#endif

		CCamera cam((SDL_Rect){0,TILESIZE, TILESIZE * 20,TILESIZE * 10}, (SDL_Rect){0,0,0,0});
		#ifndef USE_SDL2
			CPlayer player;
			CGameScreen gamescreen(screen, &cam, &player, TILESIZE);
		#else
			CPlayer player(renderer);
			CGameScreen gamescreen(window, renderer, &cam, &player, TILESIZE);
		#endif
		
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
				
				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = 1;
					else if (event.key.keysym.sym == SDLK_f)
					{
						/*
						#ifndef USE_SDL2
							fullscreen = screen->flags;
							fullscreen ^= SDL_FULLSCREEN;
							screen = SDL_SetVideoMode(screen->w, screen->h, screen->format->BitsPerPixel, fullscreen);
							if (!screen)
								throw SDL_GetError();
						#else
						*/
						#if USE_SDL2
							#if _WIN32 || _WIN64 || __MINGW32__ || !__linux__
								fullscreen ^= SDL_WINDOW_FULLSCREEN;
								SDL_SetWindowFullscreen(window, fullscreen);
							#else
								cout << "Fullscreen desativado para seu sistema, sorry!\n";
							#endif
						#endif
					}
						
				}

				gamescreen.input(event);
			}
		
			if (control_fps(time_now, fps))
			{
				gamescreen.update();
				#ifndef USE_SDL2
					gamescreen.draw();
					SDL_UpdateRect(screen, 0,0,0,0);
				#else
					SDL_RenderClear(renderer);
					gamescreen.draw();
					SDL_RenderPresent(renderer);
				#endif
			}
		}
		
		#ifdef USE_SDL2
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
		#endif
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
	CSoundPlayer::instance()->free_sounds();
	Mix_CloseAudio();

	return 0;
}

