#include "label.hpp"
#include "writer.hpp"

GuiLabel::GuiLabel ( std::string s, SDL_Color c, const std::string fontName )
{
	str = "";
	color = c;
	texture = nullptr;
	/*
		if (s != "")
			set_str(s);
		else
			str = "";
		*/
	if (s == "")
		s = " ";
	str_to_surface(s,fontName);
	this->fontName = fontName;
	if (s != "" && !texture)
			throw Exception("GuiLabel::erro ao setar texture");
	
	printf("Texture ptr = %p\n", texture);
}

GuiLabel::~GuiLabel (  )
{
	if (texture)
		SDL_DestroyTexture(texture);
	texture = nullptr;
}

void GuiLabel::str_to_surface ( std::string s, std::string fontName )
{
	int w, h;
	if (texture)
		SDL_DestroyTexture(texture);

	texture = nullptr;
	try {
		texture = Writer::instance()->render_text(fontName, s, color, SOLID_TEXT);
	} catch(Exception & e){
		e.what();
		abort();
	}

	if (!texture)
	{
		char * e = new char[256];
		sprintf(e, "GuiLabel: erro %s\n", SDL_GetError());
		throw Exception(e);
	}

	SDL_QueryTexture(texture, 0, 0, &w, &h);

	str = s;

	dim.x = pos.x + rel_pos.x, dim.y = pos.y + rel_pos.y;
	dim.w = w, dim.h = h;
}

void GuiLabel::set_color ( SDL_Color c )
{
	color = c;
	str_to_surface(str);
}

void GuiLabel::set_str ( std::string s, std::string fontName )
{
	/*
	if (s == str && texture)
		return;
	*/

	str = s;

	str_to_surface(s, fontName);
}

SDL_Texture * GuiLabel::get_texture (  )
{
	return texture;
}

void GuiLabel::set_texture ( SDL_Texture * t )
{
	if (texture && texture != t)
		SDL_DestroyTexture(texture);

	texture = t;

	if (texture)
	{
		int w, h;
		SDL_QueryTexture(texture, 0, 0, &w, &h);
		dim.w = w;
		dim.h = h;
	}
	else
	{
		dim.w = dim.h = 0;
	}
}

int GuiLabel::get_texture_width (  )
{
	int w;

	if (!texture)
		return 0;

	SDL_QueryTexture(texture, 0, 0, &w, 0);
	return w;
}

int GuiLabel::get_texture_height (  )
{
	int h;

	if (!texture)
		return 0;

	SDL_QueryTexture(texture, 0, 0, 0, &h);
	return h;
}

std::string GuiLabel::get_str (  )
{
	return str;
}

void GuiLabel::draw ( SDL_Renderer * renderer )
{
	if (!visible)
		return;

	child_draw(renderer);
	SDL_Rect d = {int(pos.x),int(pos.y),dim.w,dim.h};
	if (texture)
		SDL_RenderCopy(renderer, texture, nullptr, &d);
}

///////////////////////////////////////////////////////////

void GuiLabelNumber::proc_value ( float v )   // para ser usada internamente
{
	int n = v, a, i;
	std::string s, aux;
	value = v;

	for (i = 0; n > 0; i++)
	{
		a = n % 10;
		n = (n - a) / 10;
		aux.push_back('0' + a);
	}

	// inverte aux;
	for (i = aux.size() - 1; i > -1; i--)
		s.push_back(aux[i]);

	aux.swap(s);
	s.clear();

	if (aux.size() < left_zero)
	{
		for (i = 0; i < left_zero - aux.size(); i++)
			s.push_back('0');

		s.append(aux);
	}
	else
	{
		s.swap(aux);
	}

	set_str(s);
}

void GuiLabelNumber::set_left_zero ( int lz )
{
	if (lz > -1 && lz != left_zero)
	{
		left_zero = lz;
		proc_value(value);
	}
}

bool GuiLabelNumber::set_value ( float v )
{
	if (value == v)
		return false;

	proc_value(int(v));
	value = v;

	return true;
}

