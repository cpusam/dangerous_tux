/*
 Copyright (C) 2014 Samuel Leonardo

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

#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <string>
#include <vector>

#include "../chora.hpp"
#include "../statemachine.hpp"
#include "../vect.hpp"

class CWidget: public CStateMachine
{
	protected:
		std::string id; // identificação do widget
		SVect pos; // posição absoluta no screen
		SVect rel_pos; // posição relativa ao widget pai
		SDL_Rect dim; // dimensão, dim.x e dim.y posição no screen
		bool visible; // só recebe eventos se estiver visível
	private:
		CWidget * parent;
		std::vector <CWidget *> child;
	
	public:
		CWidget ( std::string i="" )
		{
			id = i;
			parent = 0;
			visible = true;
			set_state(1);
		}
		
		CWidget ( CWidget & w )
		{
			id = w.get_id();
			pos = w.get_pos();
			rel_pos = w.get_rel_pos();
			dim = w.get_dim();
			visible = w.is_visible();
			parent = w.get_parent();
			child.reserve(w.child.size());
			for (int i(0); i < w.child.size(); i++)
				child[i] = w.get_child(i);
		}
		
		void set_id ( std::string new_id );
		
		std::string get_id (  );
		
		void set_pos ( SVect p );
		
		SVect get_pos (  );
			
		void set_rel_pos ( SVect p );
		
		SVect get_rel_pos (  );
		
		void set_dim ( SDL_Rect d );
		
		SDL_Rect get_dim (  );
		
		void show ( bool s=true );
		
		bool is_visible (  );
		
		void set_parent ( CWidget * w );
		
		CWidget * get_parent (  );
		
		bool has_child ( CWidget * w );
		
		void show_child ( bool s=true );
		
		bool add_child ( CWidget * w );
		
		void clear_child (  );
		
		bool rem_child ( CWidget * w );
		
		CWidget * get_child ( std::string s_id );
		
		CWidget * get_child ( int index );
		
		int child_size (  );
		
		/*
			as funções child_input, child_update e child_draw devem ser chamadas nas
			respectivas funções input, update e draw isso se a classe derivada 
			de CWidget possuir filhos.
		*/
		void child_input ( SDL_Event & event );
		
		int child_update (  );
		
		#ifndef USE_SDL2
			void child_draw ( SDL_Surface * screen );
		#else
			void child_draw ( SDL_Renderer * renderer );
		#endif
		
		
		
		virtual void input ( SDL_Event & event );
		
		virtual int update (  );
		
		#ifndef USE_SDL2
			virtual void draw ( SDL_Surface * screen );
		#else
			virtual void draw ( SDL_Renderer * renderer );
		#endif
};

#endif

