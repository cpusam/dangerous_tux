#include "widget.hpp"
#include <algorithm>

void Widget::destroy (  )
{
	for (auto * it: child)
		if (it)
		{
			it->destroy();
			delete it;
		}
	child.clear();
}

void Widget::set_id ( std::string new_id )
{
	id = new_id;
}

std::string Widget::get_id (  )
{
	return id;
}

void Widget::set_pos ( Vect p )
{
	if (pos.x == p.x && pos.y == p.y)
		return;

	pos = p;
	dim.x = int(p.x), dim.y = int(p.y);

	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		(*i)->set_pos(Vect::add((*i)->get_rel_pos(), pos));
}

Vect Widget::get_pos (  )
{
	return pos;
}

void Widget::set_rel_pos ( Vect p )
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

	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		(*i)->set_pos((*i)->get_rel_pos() + pos);
}

Vect Widget::get_rel_pos (  )
{
	return rel_pos;
}

void Widget::set_dim ( SDL_Rect d )
{
	d.x = int(pos.x + rel_pos.x);
	d.y = int(pos.y + rel_pos.y);
	dim = d;
}

SDL_Rect Widget::get_dim (  )
{
	return dim;
}

void Widget::show ( bool s )
{
	visible = s;
	show_child(s);
}

bool Widget::is_visible (  )
{
	return visible;
}

void Widget::set_parent ( Widget * w )
{
	if (parent)
		parent->rem_child(this);
	parent = w;
}

Widget * Widget::get_parent (  )
{
	return parent;
}

bool Widget::has_child ( Widget * w )
{
	for (unsigned int i(0); i < child.size(); i++)
		if (w == child[i] && w)
			return true;

	return false;
}

void Widget::show_child ( bool s )
{
	if (visible)
		for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		{
			(*i)->show(s);
			(*i)->show_child(s);
		}
}

bool Widget::add_child ( Widget * w )
{
	if (!has_child(w))
	{
		w->set_parent(this);
		child.push_back(w);
		return true;
	}

	return false;
}

void Widget::clear_child (  )
{
	child.clear();
}

bool Widget::rem_child ( Widget * w )
{
	if (has_child(w))
	{
		for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
			if (*i == w)
			{
				child.erase(i);
				return true;
			}
	}

	return false;
}

Widget * Widget::get_child ( std::string s_id )
{
	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->get_id() == s_id)
			return (*i);

	return 0;
}

Widget * Widget::get_child ( int index )
{
	if (index > -1 && index < (int)child.size())
		return child.at(index);

	return 0;
}

const std::vector<Widget *> Widget::get_all_children()
{
	std::vector<Widget *> all = this->child;
	
	for (size_t i = 0, end = all.size(); i < end; ++i)
	{
		std::vector<Widget *> children = all[i]->get_children();
		for (Widget * child: children)
			if (child && std::find(all.begin(), all.end(), child) == all.end())
				all.push_back(child);
	}
	
	return all;
}

const std::vector<Widget *> & Widget::get_children (  )
{
	return child;
}

int Widget::child_size (  )
{
	return child.size();
}

/*
	as funções child_input, child_update e child_draw devem ser chamadas nas
	respectivas funções input, update e draw isso se a classe derivada
	de Widget possuir filhos.
*/
void Widget::child_input ( SDL_Event & event )
{
	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->is_visible() && visible)
			(*i)->input(event);
}

int Widget::child_update (  )
{
	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->is_visible() && visible)
			(*i)->update();

	return DEFAULT_STATE;
}


void Widget::child_draw ( SDL_Renderer * renderer )
{
	for (std::vector <Widget *>::iterator i = child.begin(), end = child.end(); i != end; i++)
		if ((*i)->is_visible() && visible)
			(*i)->draw(renderer);
}


void Widget::input ( SDL_Event & event )
{
	child_input(event);
}

int Widget::update (  )
{
	child_update();

	return get_state();
}

void Widget::draw ( SDL_Renderer * renderer )
{
	child_draw(renderer);
}


