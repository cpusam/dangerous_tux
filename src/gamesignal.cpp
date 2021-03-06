#include "gamesignal.hpp"

CKernelSignal::CKernelSignal ( SDL_Renderer * r )
{
	SDL_Surface * aux = 0;
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path, "%s\\images\\kernel_signal.png", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\images\\kernel_signal.png", PREFIX);
		#endif
	#else
		char path[1024];
		#ifndef PREFIX
			sprintf(path, "./images/kernel_signal.png");
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/kernel_signal.png", PREFIX);
		#endif
	#endif

	SDL_Texture * texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CKernelSignal: não foi possível abrir kernel_signal.png\n";

	anim.add_frame(texture, (SDL_Rect){0,0,48,48}, 60);
	anim.add_frame(texture, (SDL_Rect){0,48,48,48}, 60);
	anim.add_frame(texture, (SDL_Rect){0,48*2,48,48}, 60);
	anim.add_frame(texture, (SDL_Rect){0,48*3,48,48}, 60);
	show(false);
	cam = 0;
}

CKernelSignal::~CKernelSignal (  )
{
	anim.destroy_textures();
}
	
void CKernelSignal::set_cam ( Camera * c )
{
	cam = c;
	
	if (!cam)
		throw "CKernelSignal: camera nula\n";
}

void CKernelSignal::set_kernel_pos ( Vect kp )
{
	kernel_pos = Vect(kp.x + 48/2, kp.y + 48/2);
}

void CKernelSignal::show ( bool s )
{
	visible = s;
	set_state(2);
}

void CKernelSignal::draw ( SDL_Renderer * renderer )
{
	if (!cam)
		throw "CKernelSignal: camera nula\n";

	if (visible)
		anim.draw(renderer, cam, pos.x, pos.y);
}

int CKernelSignal::update (  )
{
	switch (get_state())
	{
		case 0: // inativo
			break;
		
		case 1: // executando
			{
				if (!cam)
					throw "CKernelSignal: camera nula\n";

				SDL_Rect d = cam->get_dimension();
				Vect pcam = cam->get_position();
				
				if (pos.x < kernel_pos.x)
					pos.x = pcam.x + d.w - 48/2;
				else
					pos.x = pcam.x - 48/2;

				pos.y = kernel_pos.y - 48/2;
			
				d.x = pcam.x;
				d.y = pcam.y;
				if (pointbox(kernel_pos, d))
				{
					visible = false;
					set_state(2);
					break;
				}

				anim.update();
			}
			break;
		
		case 2: // esperando
			{
				if (!cam)
					throw "CKernelSignal: camera nula\n";
				
				SDL_Rect d = cam->get_dimension();
				d.x = cam->get_position().x;
				d.y = cam->get_position().y;
				
				if (!pointbox(kernel_pos, d))
				{
					visible = true;
					set_state(1);
				}
				else
				{
					visible = false;
				}
				
			}
			break;
	}
	
	return get_state();
}

//////////////////////////////////////////////////////////////////

CExitSignal::CExitSignal ( SDL_Renderer * r )
{
	SDL_Surface * aux = 0;
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path, "%s\\images\\exit_signal.png", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\images\\exit_signal.png", PREFIX);
		#endif
	#else
		char path[1024];
		#ifndef PREFIX
			sprintf(path, "./images/exit_signal.png");
		#else
			sprintf(path, "%s/share/games/dangeroustux/images/exit_signal.png", PREFIX);
		#endif
	#endif
	SDL_Texture * texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CExitSignal: não foi possível abrir exit_signal.png\n";

	anim.add_frame(texture, (SDL_Rect){0,0,48,48}, 60);
	anim.add_frame(texture, (SDL_Rect){0,48,48,48}, 60);
	anim.add_frame(texture, (SDL_Rect){0,48*2,48,48}, 60);
	anim.add_frame(texture, (SDL_Rect){0,48*3,48,48}, 60);
	show(false);
	cam = 0;
}

CExitSignal::~CExitSignal (  )
{
	anim.destroy_textures();
}

void CExitSignal::set_cam ( Camera * c )
{
	cam = c;
	if (!cam)
		throw "CExitSignal: camera nula\n";
}

void CExitSignal::set_exit_pos ( Vect ep )
{
	exit_pos = Vect(ep.x + 48/2, ep.y + 48/2);
}

void CExitSignal::show ( bool s )
{
	visible = s;
	set_state(2);
}

void CExitSignal::draw ( SDL_Renderer * renderer )
{
	if (!cam)
		throw "CExitSignal: camera nula\n";

	if (visible)
		anim.draw(renderer, cam, pos.x, pos.y);
}

int CExitSignal::update (  )
{
	switch (get_state())
	{
		case 0: // inativo
			break;
		
		case 1: // executando
			{
				if (!cam)
					throw "CExitSignal: camera nula\n";

				SDL_Rect d = cam->get_dimension();
				Vect pcam = cam->get_position();
				Vect ccam = Vect(pcam.x + d.w/2, pcam.y + d.h/2);
				
				if (pos.x < exit_pos.x)
					pos.x = pcam.x + d.w - 48/2;
				else
					pos.x = pcam.x - 48/2;

				pos.y = exit_pos.y - 48/2;
			
				d.x = pcam.x;
				d.y = pcam.y;
				if (pointbox(exit_pos, d))
				{
					visible = false;
					set_state(2);
					break;
				}

				anim.update();
			}
			break;
		
		case 2: // esperando
			{
				if (!cam)
					throw "CExitSignal: camera nula\n";
		
				SDL_Rect d = cam->get_dimension();
				d.x = cam->get_position().x;
				d.y = cam->get_position().y;
				
				if (!pointbox(exit_pos, d))
				{
					visible = true;
					set_state(1);
				}
				else
				{
					visible = false;
				}
			}
			break;
	}
	
	return get_state();
}


