#ifndef WIDGET_HPP
#define WIDGET_HPP

using namespace std;

class CWidget: public CStateMachine
{
	protected:
		string id; // identificação do widget
		SVect pos; // posição absoluta no screen
		SVect rel_pos; // posição relativa ao widget pai
		SDL_Rect dim; // dimensão, dim.x e dim.y posição no screen
		bool visible; // só recebe eventos se estiver visível
	private:
		CWidget * parent;
		vector <CWidget *> child;
	
	public:
		CWidget ( string i="" )
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
		
		/*
			as funções child_input, child_update e child_draw devem ser chamadas nas
			respectivas funções input, update e draw isso se a classe derivada 
			de CWidget possuir filhos.
		*/
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
		#endif
		
		
		
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
		#endif
};

#endif

