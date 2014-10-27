#include "widget.hpp"

void CWidget::set_id ( std::string new_id )
{
	id = new_id;
}

std::string CWidget::get_id (  )
{
	return id;
}		

void CWidget::set_pos ( SVect p )
{
	if (pos.x == p.x && pos.y == p.y)
		return;

	pos = p;
	dim.x = int(p.x), dim.y = int(p.y);
	
	for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
		(*i)->set_pos((*i)->get_rel_pos() + pos);
}

SVect CWidget::get_pos (  )
{
	return pos;
}
	
void CWidget::set_rel_pos ( SVect p )
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
	
	for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
		(*i)->set_pos((*i)->get_rel_pos() + pos);
}

SVect CWidget::get_rel_pos (  )
{
	return rel_pos;
}

void CWidget::set_dim ( SDL_Rect d )
{
	d.x = int(pos.x + rel_pos.x);
	d.y = int(pos.y + rel_pos.y);
	dim = d;
}

SDL_Rect CWidget::get_dim (  )
{
	return dim;
}

void CWidget::show ( bool s )
{
	visible = s;
	show_child(s);
}

bool CWidget::is_visible (  )
{
	return visible;
}

void CWidget::set_parent ( CWidget * w )
{
	if (parent)
		parent->rem_child(this);
	parent = w;
}

CWidget * CWidget::get_parent (  )
{
	return parent;
}

bool CWidget::has_child ( CWidget * w )
{
	for (int i(0); i < child.size(); i++)
		if (w == child[i] && w)
			return true;
	
	return false;
}

void CWidget::show_child ( bool s )
{
	if (visible)
		for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
		{
			(*i)->show(s);
			(*i)->show_child(s);
		}
}

bool CWidget::add_child ( CWidget * w )
{
	if (!has_child(w))
	{
		w->set_parent(this);
		child.push_back(w);
		return true;
	}
	
	return false;
}

void CWidget::clear_child (  )
{
	child.clear();
}

bool CWidget::rem_child ( CWidget * w )
{
	if (has_child(w))
	{
		for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
			if (*i == w)
			{
				child.erase(i);
				return true;
			}
	}
	
	return false;
}

CWidget * CWidget::get_child ( std::string s_id )
{
	for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->get_id() == s_id)
			return (*i);

	return 0;
}

CWidget * CWidget::get_child ( int index )
{
	if (index > -1 && index < child.size())
		return child.at(index);
	
	return 0;
}

int CWidget::child_size (  )
{
	return child.size();
}

/*
	as funções child_input, child_update e child_draw devem ser chamadas nas
	respectivas funções input, update e draw isso se a classe derivada 
	de CWidget possuir filhos.
*/
void CWidget::child_input ( SDL_Event & event )
{
	for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->is_visible() && visible)
			(*i)->input(event);
}

int CWidget::child_update (  )
{
	for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->is_visible() && visible)
			(*i)->update();
	
	return DEFAULT_STATE;
}

#ifndef USE_SDL2
	void CWidget::child_draw ( SDL_Surface * screen )
	{
		for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
			if ((*i)->is_visible() && visible)
				(*i)->draw(screen);
	}
#else
	void CWidget::child_draw ( SDL_Renderer * renderer )
	{
		for (std::vector <CWidget *>::iterator i = child.begin(); i != child.end(); i++)
			if ((*i)->is_visible() && visible)
				(*i)->draw(renderer);
	}
#endif



void CWidget::input ( SDL_Event & event )
{
	child_input(event);
}

int CWidget::update (  )
{
	child_update();
	
	return get_state();
}

#ifndef USE_SDL2
	void CWidget::draw ( SDL_Surface * screen )
	{
		child_draw(screen);
	}
#else
	void CWidget::draw ( SDL_Renderer * renderer )
	{
		child_draw(renderer);
	}
#endif


