#include "video.hpp"

SDL_Surface * set_screen ( int w, int h )
{
	const SDL_VideoInfo * info = SDL_GetVideoInfo();
	Uint32 flags = SDL_ANYFORMAT;
	
	if (info->hw_available && info->video_mem > 20000)
		flags |= SDL_HWSURFACE;
	else
		flags |= SDL_SWSURFACE;

	if (info->wm_available == 0)
		flags |= SDL_FULLSCREEN;
	
	flags |= SDL_SRCALPHA;
	SDL_Surface * screen = SDL_SetVideoMode(w, h, 32/*info->vfmt->BitsPerPixel*/, flags);
	SDL_SetAlpha(screen, SDL_SRCALPHA, 0);

	return screen;
}


