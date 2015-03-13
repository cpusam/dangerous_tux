#include <SDL2/SDL.h>

int main ( int argc, char **argv )
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
		throw SDL_GetError();
	
	SDL_Event event;
	SDL_Window * window = SDL_CreateWindow("teste", SDL_WINDOWPOS_CENTERED, 
	                                                SDL_WINDOWPOS_CENTERED, 
	                                                400, 300, 
	                                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		
	//SDL_Texture * target_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TILESIZE * 20, TILESIZE * 13);
		
		
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // em fase de testes
	SDL_RenderSetLogicalSize(renderer, 400, 300);

	int done = 0;

	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
		}

		SDL_SetRenderDrawColor(renderer, 255,255,0,255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		//SDL_Delay(60);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}
