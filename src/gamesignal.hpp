#ifndef GAMESIGNAL_HPP
#define GAMESIGNAL_HPP

<<<<<<< HEAD
#include "gameentity.hpp"
#include "chora_engine/collision.hpp"
#include "chora_engine/animation.hpp"
#include "chora_engine/util.hpp"

=======
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
class CKernelSignal: public CGameEntity
{
	protected:
		CAnimation anim;
		CCamera * cam;
		SVect kernel_pos;
		bool visible;
	
	public:
		#ifndef USE_SDL2
<<<<<<< HEAD
			CKernelSignal (  );
		#else
			CKernelSignal ( SDL_Renderer * r );
		#endif
		
		virtual ~CKernelSignal (  );
			
		void set_cam ( CCamera * c );
		
		void set_kernel_pos ( SVect kp );
		
		void show ( bool s=true );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
		int update (  );
=======
			CKernelSignal (  )
		#else
			CKernelSignal ( SDL_Renderer * r )
		#endif
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

			#ifndef USE_SDL2
				aux = optimize_surface_alpha(IMG_Load(path));
				if (!aux)
					throw "CKernelSignal: não foi possível abrir kernel_signal.png\n";
			#else
				SDL_Texture * texture = IMG_LoadTexture(r, path);
				if (!texture)
					throw "CKernelSignal: não foi possível abrir kernel_signal.png\n";
			#endif
			#ifndef USE_SDL2
				anim.add_frame(aux, (SDL_Rect){0,0,48,48}, 6);
				anim.add_frame(aux, (SDL_Rect){0,48,48,48}, 6);
				anim.add_frame(aux, (SDL_Rect){0,48*2,48,48}, 6);
				anim.add_frame(aux, (SDL_Rect){0,48*3,48,48}, 6);
			#else
				anim.add_frame(texture, (SDL_Rect){0,0,48,48}, 6);
				anim.add_frame(texture, (SDL_Rect){0,48,48,48}, 6);
				anim.add_frame(texture, (SDL_Rect){0,48*2,48,48}, 6);
				anim.add_frame(texture, (SDL_Rect){0,48*3,48,48}, 6);
			#endif
			show(false);
			cam = 0;
		}
		
		~CKernelSignal (  )
		{
			#ifndef USE_SDL2
				anim.destroy_surfaces();
			#else
				anim.destroy_textures();
			#endif
		}
			
		void set_cam ( CCamera * c )
		{
			cam = c;
			
			if (!cam)
				throw "CKernelSignal: camera nula\n";
		}
		
		void set_kernel_pos ( SVect kp )
		{
			kernel_pos = SVect(kp.x + 48/2, kp.y + 48/2);
		}
		
		void show ( bool s=true )
		{
			visible = s;
			set_state(2);
		}
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen )
		#else
			void draw ( SDL_Renderer * renderer )
		#endif
		{
			if (!cam)
				throw "CKernelSignal: camera nula\n";

			if (visible)
				#ifndef USE_SDL2
					anim.draw(pos.x, pos.y, cam, screen);
				#else
					anim.draw(pos.x, pos.y, cam, renderer);
				#endif
		}
		
		int update (  )
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
						SVect pcam = cam->get_position();
						
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
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

class CExitSignal: public CGameEntity
{
	protected:
		CAnimation anim;
		CCamera * cam;
		SVect exit_pos;
		bool visible;
	
	public:
		#ifndef USE_SDL2
<<<<<<< HEAD
			CExitSignal (  );
		#else
			CExitSignal ( SDL_Renderer * r );
		#endif
		
		virtual ~CExitSignal (  );
		
		void set_cam ( CCamera * c );
		
		void set_exit_pos ( SVect ep );
		
		void show ( bool s=true );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
		int update (  );
=======
			CExitSignal (  )
		#else
			CExitSignal ( SDL_Renderer * r )
		#endif
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

			#ifndef USE_SDL2
				aux = optimize_surface_alpha(IMG_Load(path));
				if (!aux)
					throw "CExitSignal: não foi possível abrir exit_signal.png\n";
			#else
				SDL_Texture * texture = IMG_LoadTexture(r, path);
				if (!texture)
					throw "CExitSignal: não foi possível abrir exit_signal.png\n";
			#endif
			
			#ifndef USE_SDL2
				anim.add_frame(aux, (SDL_Rect){0,0,48,48}, 6);
				anim.add_frame(aux, (SDL_Rect){0,48,48,48}, 6);
				anim.add_frame(aux, (SDL_Rect){0,48*2,48,48}, 6);
				anim.add_frame(aux, (SDL_Rect){0,48*3,48,48}, 6);
			#else
				anim.add_frame(texture, (SDL_Rect){0,0,48,48}, 6);
				anim.add_frame(texture, (SDL_Rect){0,48,48,48}, 6);
				anim.add_frame(texture, (SDL_Rect){0,48*2,48,48}, 6);
				anim.add_frame(texture, (SDL_Rect){0,48*3,48,48}, 6);
			#endif
			show(false);
			cam = 0;
		}
		
		~CExitSignal (  )
		{
			#ifndef USE_SDL2
				anim.destroy_surfaces();
			#else
				anim.destroy_textures();
			#endif
		}
		
		void set_cam ( CCamera * c )
		{
			cam = c;
			if (!cam)
				throw "CExitSignal: camera nula\n";
		}
		
		void set_exit_pos ( SVect ep )
		{
			exit_pos = SVect(ep.x + 48/2, ep.y + 48/2);
		}
		
		void show ( bool s=true )
		{
			visible = s;
			set_state(2);
		}
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen )
		#else
			void draw ( SDL_Renderer * renderer )
		#endif
		{
			if (!cam)
				throw "CExitSignal: camera nula\n";

			if (visible)
				#ifndef USE_SDL2
					anim.draw(pos.x, pos.y, cam, screen);
				#else
					anim.draw(pos.x, pos.y, cam, renderer);
				#endif
		}
		
		int update (  )
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
						SVect pcam = cam->get_position();
						SVect ccam = SVect(pcam.x + d.w/2, pcam.y + d.h/2);
						
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
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

#endif
