#ifndef GAMEVIDEO_HPP
#define GAMEVIDEO_HPP

SDL_Surface * set_screen ( int w, int h, int * fps )
{
	const SDL_VideoInfo * info = SDL_GetVideoInfo();
	Uint32 flags = SDL_ANYFORMAT;
	int bpp = 32;
	
	if (info->hw_available && info->video_mem > 20000)
		flags |= SDL_HWSURFACE;
	else
		flags |= SDL_SWSURFACE;

	if (info->wm_available == 0)
		flags |= SDL_FULLSCREEN;
	
	bpp = info->vfmt->BitsPerPixel;
	
	switch (bpp)
	{
		case 8:
			*fps = 160;
			break;
		case 16:
			*fps = 80;
			break;
		case 24:
			*fps = 75;
			break;
		case 32:
			*fps = 50;
			break;
		default:
			throw "set_video(): não foi possível detectar bits per pixel\n";
			break;
	}
	
	cout << "set_video() usando fps de " << *fps << endl;
	
	return SDL_SetVideoMode(w, h, bpp, flags);
}

#endif

