/*
 Copyright (C) 2014 Samuel Leonardo and Gustavo Medeiros

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#if _WIN32 || _WIN64 
	#include <windows.h>
	#include <direct.h>
#else
	#include <unistd.h>
#endif

#include <iostream>
#include <exception>
#include <ctime>

#include "chora_engine/chora.hpp"
#include "chora_engine/SDL_gfx/SDL_framerate.hpp"

#include "gamescreen.hpp" // contém todos os headers do jogo

#ifndef USE_SDL2
	#include "video.hpp"
#endif

#define TILESIZE 48

int main ( int argc, char **argv )
{
	try
	{
		#ifndef USE_SDL2
			SDL_putenv("SDL_VIDEO_CENTERED=center");
		#else
			#if _WIN32 || _WIN64
				SDL_SetMainReady();
			#endif
		#endif

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0)
			throw SDL_GetError();
	
		if (TTF_Init() < 0)
			throw SDL_GetError();

		if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
			throw SDL_GetError();
		
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
			throw SDL_GetError();
		
		Mix_AllocateChannels(20); // aloca 20 canais para tocar sons

		SDL_Event event;

		int fullscreen = 0;
		#ifndef USE_SDL2
			// no tamanho aproximado do dangerous dave original
			SDL_Surface * screen = set_screen(TILESIZE * 20, TILESIZE * 13);
			if (!screen)
				throw SDL_GetError();
			SDL_WM_SetCaption("Dangerous Tux - beta version 2014", NULL);
		#else
			SDL_Window * window = SDL_CreateWindow("Dangerous Tux! BETA version 2014", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TILESIZE * 20, TILESIZE * 13, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
			SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			
			SDL_Texture * target_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TILESIZE * 20, TILESIZE * 13);
			
			
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // em fase de testes
			SDL_RenderSetLogicalSize(renderer, TILESIZE * 20, TILESIZE * 13);
		#endif

		CCamera cam((SDL_Rect){0,0, TILESIZE * 20,TILESIZE * 13}, (SDL_Rect){0,0,0,0});
		#ifndef USE_SDL2
			CGameScreen gamescreen(screen, &cam, TILESIZE);
		#else
			CWriter::instance()->set_renderer(renderer);
			CGameScreen gamescreen(window, renderer, &cam, TILESIZE);
		#endif
		
		srand(time(0));
		
		int done = 0;
		FPSManager::instance()->set_framerate(40);
		while (!done)
		{
			//time_now = SDL_GetTicks();
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
							#if _WIN32 || _WIN64 || !__linux__
								fullscreen ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
								SDL_SetWindowFullscreen(window, fullscreen);
							#else
								cout << "Fullscreen desativado para seu sistema, sorry!\n";
							#endif
						#endif
					}
				}

				gamescreen.input(event);
			}

			FPSManager::instance()->update();
			
			if (FPSManager::instance()->get_delta())
			{
				if (gamescreen.update() == EXIT_SCREEN)
					done = 1;
				#ifndef USE_SDL2
					gamescreen.draw();
					SDL_UpdateRect(screen, 0,0,0,0);
				#else
					SDL_SetRenderTarget(renderer, target_texture);
					gamescreen.draw();
					SDL_RenderCopy(renderer, target_texture, NULL, NULL);
					SDL_SetRenderTarget(renderer, NULL);
				
					//SDL_RenderPresent(renderer);
				
					SDL_SetRenderDrawColor(renderer, 0,0,0,255);
					SDL_RenderClear(renderer);
					SDL_RenderCopyEx(renderer, target_texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
					SDL_RenderPresent(renderer);
				#endif
			}
		}
		
		#ifdef USE_SDL2
			SDL_DestroyTexture(target_texture);
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
		#endif
	}
	catch (const char * e)
	{
		cout << "Erro: " << e << endl;
		#ifdef USE_SDL2
			SDL_Log(e);
		#endif
		return 1;
	}
	// Erros com alguma lib SDL
	catch (char * e)
	{
		cout << "Erro: " << e << endl;
		#ifdef USE_SDL2
			SDL_Log(e);
		#endif
		return 1;
	}
	catch (exception & e)
	{
		cout << e.what() << endl;
		#ifdef USE_SDL2
			SDL_Log(e.what());
		#endif
		return 1;
	}

	IMG_Quit();
	TTF_Quit();
	CSoundPlayer::instance()->free_sounds();
	Mix_CloseAudio();
	SDL_Quit();
	
	return 0;
}

