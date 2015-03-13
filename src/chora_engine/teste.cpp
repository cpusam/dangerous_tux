#include <iostream>
#include <SDL/SDL.h>
#include <emscripten/emscripten.h>

using namespace std;

struct Data
{
	SDL_Surface * screen;
};

void main_loop ( void * arg )
{
	Data * d = (Data *)arg;
	
	SDL_FillRect(d->screen, NULL, SDL_MapRGB(d->screen->format, 255,255,0));
	SDL_Flip(d->screen);
}

int main(int argc, char* argv[]) {
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << SDL_GetError() << endl;
		return 1;
	}
	
	SDL_Event event;
	SDL_Surface * screen = SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
	if (!screen)
	{
		cout << SDL_GetError() << endl;
		return 1;
	}

	Data data = {screen};

	emscripten_set_main_loop_arg(main_loop, &data, 60, 1);
	
	cout << "limpando tudo\n";
	SDL_Quit();

  return 0;
}
