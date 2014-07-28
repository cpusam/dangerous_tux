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

<<<<<<< HEAD
#include <string>
#include <vector>

#include "../chora.hpp"
#include "../statemachine.hpp"
#include "../vect.hpp"
=======
using namespace std;
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813

class CWidget: public CStateMachine
{
	protected:
<<<<<<< HEAD
		std::string id; // identificação do widget
=======
		string id; // identificação do widget
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		SVect pos; // posição absoluta no screen
		SVect rel_pos; // posição relativa ao widget pai
		SDL_Rect dim; // dimensão, dim.x e dim.y posição no screen
		bool visible; // só recebe eventos se estiver visível
	private:
		CWidget * parent;
<<<<<<< HEAD
		std::vector <CWidget *> child;
	
	public:
		CWidget ( std::string i="" )
=======
		vector <CWidget *> child;
	
	public:
		CWidget ( string i="" )
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
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
		
<<<<<<< HEAD
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
=======
		void set_id ( string new_id )
		{
			id = new_id;
		}
		
		string get_id (  )
		{
			return id;
		}		
		
		void set_pos ( SVect p )
		{
			if (pos.x == p.x && pos.y == p.y)
				return;

			pos = p;
			dim.x = int(p.x), dim.y = int(p.y);
			
			for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
				(*i)->set_pos((*i)->get_rel_pos() + pos);
		}
		
		SVect get_pos (  )
		{
			return pos;
		}
			
		void set_rel_pos ( SVect p )
		{
			if (rel_pos.x == p.x && rel_pos.y == p.y)
				return;

			rel_pos = p;
			
			if (parent)
			{
				pos.x = parent->get_pos().x + rel_pos.x;
				pos.y = parent->get_pos().y + rel_pos.y;
				dim.x = int(pos.x);
				dim.y = int(pos.y);
			}
			else
			{
				dim.x = int(rel_pos.x);
				dim.y = int(rel_pos.y);
				pos = rel_pos;
			}
			
			for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
				(*i)->set_pos((*i)->get_rel_pos() + pos);
		}
		
		SVect get_rel_pos (  )
		{
			return rel_pos;
		}
		
		void set_dim ( SDL_Rect d )
		{
			d.x = int(pos.x + rel_pos.x);
			d.y = int(pos.y + rel_pos.y);
			dim = d;
		}
		
		SDL_Rect get_dim (  )
		{
			return dim;
		}
		
		void show ( bool s=true )
		{
			visible = s;
			show_child(s);
		}
		
		bool is_visible (  )
		{
			return visible;
		}
		
		void set_parent ( CWidget * w )
		{
			if (parent)
				parent->rem_child(this);
			parent = w;
		}
		
		CWidget * get_parent (  )
		{
			return parent;
		}
		
		bool has_child ( CWidget * w )
		{
			for (int i(0); i < child.size(); i++)
				if (w == child[i] && w)
					return true;
			
			return false;
		}
		
		void show_child ( bool s=true )
		{
			if (visible)
				for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
					(*i)->show(s);
		}
		
		bool add_child ( CWidget * w )
		{
			if (!has_child(w))
			{
				w->set_parent(this);
				child.push_back(w);
				return true;
			}
			
			return false;
		}
		
		void clear_child (  )
		{
			child.clear();
		}
		
		bool rem_child ( CWidget * w )
		{
			if (has_child(w))
			{
				for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
					if (*i == w)
					{
						child.erase(i);
						return true;
					}
			}
			
			return false;
		}
		
		CWidget * get_child ( string s_id )
		{
			for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
				if ((*i)->get_id() == s_id)
					return (*i);

			return 0;
		}
		
		CWidget * get_child ( int index )
		{
			if (index > -1 && index < child.size())
				return child.at(index);
			
			return 0;
		}
		
		int child_size (  )
		{
			return child.size();
		}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		
		/*
			as funções child_input, child_update e child_draw devem ser chamadas nas
			respectivas funções input, update e draw isso se a classe derivada 
			de CWidget possuir filhos.
		*/
<<<<<<< HEAD
		void child_input ( SDL_Event & event );
		
		int child_update (  );
		
		#ifndef USE_SDL2
			void child_draw ( SDL_Surface * screen );
		#else
			void child_draw ( SDL_Renderer * renderer );
=======
		void child_input ( SDL_Event & event )
		{
			for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
				if ((*i)->is_visible() && visible)
					(*i)->input(event);
		}
		
		int child_update (  )
		{
			for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
				if ((*i)->is_visible() && visible)
					(*i)->update();
		}
		
		#ifndef USE_SDL2
			void child_draw ( SDL_Surface * screen )
			{
				for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
					if ((*i)->is_visible() && visible)
						(*i)->draw(screen);
			}
		#else
			void child_draw ( SDL_Renderer * renderer )
			{
				for (vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
					if ((*i)->is_visible() && visible)
						(*i)->draw(renderer);
			}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		#endif
		
		
		
<<<<<<< HEAD
		virtual void input ( SDL_Event & event );
		
		virtual int update (  );
		
		#ifndef USE_SDL2
			virtual void draw ( SDL_Surface * screen );
		#else
			virtual void draw ( SDL_Renderer * renderer );
=======
		virtual void input ( SDL_Event & event )
		{
			child_input(event);
		}
		
		virtual int update (  )
		{
			child_update();
			
			return get_state();
		}
		
		#ifndef USE_SDL2
			virtual void draw ( SDL_Surface * screen )
			{
				child_draw(screen);
			}
		#else
			virtual void draw ( SDL_Renderer * renderer )
			{
				child_draw(renderer);
			}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		#endif
};

#endif

