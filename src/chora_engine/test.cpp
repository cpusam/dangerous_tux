#include <iostream>
#include <exception>
#include <ctime>
#include <cstdlib>

#include "chora.hpp"

int main (  )
{
	try
	{
		srand(time(NULL));
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
			throw SDL_GetError();
		
		SDL_Event event;
		SDL_Surface * screen = SDL_SetVideoMode(900, 600, 32, SDL_SWSURFACE);
		if (!screen)
			throw SDL_GetError();
		
		
		SDL_Surface * aux = optimize_surface(IMG_Load("../../images/chora_logo.png"));
		if (!aux)
			throw SDL_GetError();
		
		CCollisionFrame c;
		c.set_delay(4);
		c.set_source((SDL_Rect){0,0,aux->w,aux->h});
		CSprite anim;
		SVect pos(200,0);
		anim.add_frame(aux, c);
		anim.flip(FLIP_HOR);

		//SDL_Surface * left = mirror_surface(aux);
		//SDL_Surface * left = clone_surface(aux, c.get_source());
		SDL_Surface * left = mirror_surface(aux, FLIP_HOR);//clone_surface(aux, c.get_source());//mirror_surface(clone_surface(aux, c.get_source()), FLIP_HOR);
		if (!left)
			printf("left é nulo\n");

		printf("left.w.h = %d, %d\n", left->w, left->h);
		FPSManager::instance()->set_framerate(3);
		
		int kdown = 0;
		
		int done = 0;
		while (!done)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					done = 1;
				
				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = 1;
				}

				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_DOWN)
						kdown = 1;
				}
				else if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_DOWN)
						kdown = 0;
				}

			}
			
			if (kdown)
				pos.x += 5;
			
			FPSManager::instance()->update();
			
			if (FPSManager::instance()->get_delta())
			{
				SDL_FillRect(screen, NULL, 0xFF0000);
				anim.draw(pos.x, pos.y, screen);
				SDL_Rect d = {pos.x, pos.y, 0,0};
				SDL_BlitSurface(aux, NULL, screen, &d);
				//SDL_BlitSurface(aux, NULL, screen,NULL);
				//SDL_BlitSurface(anim.get_surface(0), NULL, screen, NULL);

				SDL_UpdateRect(screen, 0,0,0,0);
				printf("delta = %u ms\n", FPSManager::instance()->get_delta());
			}
		}

		SDL_FreeSurface(aux);
		SDL_FreeSurface(left);
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (char * e)
	{
		std::cout << "Erro: " << e << std::endl;
	}
	catch (const char * e)
	{
		std::cout << "Erro: " << e << std::endl;
	}
	
	
	SDL_Quit();
	
	return 0;
}


