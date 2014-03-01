#ifndef GAMEVIDEO_HPP
#define GAMEVIDEO_HPP

SDL_Surface * set_screen ( int w, int h )
{
	const SDL_VideoInfo * info = SDL_GetVideoInfo();
	Uint32 flags = 0;
	int bpp = 32;
	
	if (info->hw_available && info->video_mem > 20000)
		flags |= SDL_HWSURFACE;
	else
		flags |= SDL_SWSURFACE;

	if (info->wm_available == 0)
		flags |= SDL_FULLSCREEN;
	
	bpp = 32; //info->vfmt->BitsPerPixel;
	cout << "set_screen bpp = " << bpp << endl; 
	
	return SDL_SetVideoMode(w, h, bpp, flags);
}

#endif

