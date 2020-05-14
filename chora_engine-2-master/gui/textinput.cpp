#include "textinput.hpp"


GuiTextInput::GuiTextInput ( int fontsize, SDL_Color c, int ss, std::string fontName ): GuiLabel("", c, fontName)
{
	if (fontsize < 1)
		throw Exception("GuiTextInput: tamanho do texto inválido\n");


	caps_lock = false;
	shift_key = false;
	timer.start();
	time_change = 200;

	str_to_surface("a", fontName);
	textsize = get_texture_width();
	int width = textsize;
	int height = get_texture_height();
	str_to_surface("", fontName);
	str = "";

	locked = false;
	remakeTexture = false;
	strsize = ss;
	cursorStart = int(str.length());
	cursor.x = pos.x + rel_pos.x;
	cursor.y = pos.y + rel_pos.y;
	cursor.w = 5;
	cursor.h = fontsize;
	cursor_color = 0xFF0000FF;

	controls["backspace"] = Key(SDLK_BACKSPACE);
	controls["delete"] = Key(SDLK_DELETE);
	controls["right"] = Key(SDLK_RIGHT);
	controls["left"] = Key(SDLK_LEFT);

	box.x = box.y = 0;
	box.w = width * ss;
	box.h = height;

	show(false);
}

GuiTextInput::~GuiTextInput (  )
{

}

SDL_Rect GuiTextInput::get_box (  )
{
	return box;
}

void GuiTextInput::show ( bool s )
{
	Widget::show(s);
	if (s)
	{
		timer.start();
		SDL_StartTextInput();
	}
	else
	{
		timer.pause();
	}
}

void GuiTextInput::lock (  )
{
	locked = true;
}

void GuiTextInput::unlock (  )
{
	locked = false;
}

void GuiTextInput::set_cursor_size ( int w, int h )
{
	cursor.w = w;
	cursor.h = h;
}

void GuiTextInput::set_pos ( Vect p )
{
	Widget::set_pos(p);
	cursor.x = pos.x + rel_pos.x + get_texture_width();
	cursor.y = pos.y + rel_pos.y;

	box.x = pos.x + rel_pos.x;
	box.y = pos.y + rel_pos.y;
}

void GuiTextInput::set_rel_pos ( Vect p )
{
	Widget::set_rel_pos(p);
	cursor.x = pos.x + rel_pos.x + get_texture_width();
	cursor.y = pos.y + rel_pos.y;

	box.x = pos.x + rel_pos.x;
	box.y = pos.y + rel_pos.y;
}

void GuiTextInput::input ( SDL_Event & event )
{
	if (!visible)
		return;
	
	for (auto & key: controls)
	{
		key.second.input(event);
	}

	if (event.type == SDL_TEXTINPUT)
	{
		if (is_visible() && locked == false)
		{
			remakeTexture = true;
			int oldSize = int(str.length());
			
			str = str + event.text.text;

			if (int(str.length()) >= strsize && strsize > 0)
			{
				str.erase(str.begin() + strsize-1, str.end());
			}

			if (cursorStart == oldSize && int(str.length()) < strsize)
				cursorStart = int(str.length());
			cursor.x = get_pos().x + cursorStart * textsize;
		}
	}

	if (event.type == SDL_TEXTEDITING)
	{
		if (is_visible() && locked == false)
		{
			remakeTexture = true;
			cursor.x = get_pos().x + cursorStart * textsize;
		}
	}

	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
			shift_key = false;
	}

	if (event.type == SDL_MOUSEMOTION)
	{
		cursorPos.x = float(event.motion.x);
		cursorPos.y = float(event.motion.y);
	}

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		cursorPos.x = float(event.button.x);
		cursorPos.y = float(event.button.y);
	}

	child_input(event);
}

void GuiTextInput::draw ( SDL_Renderer * renderer )
{
	if (!visible)
		return;

	GuiLabel::draw(renderer);

	SDL_Rect d;

	if (timer.steps() < time_change)
	{
		d = cursor;
		SDL_SetRenderDrawColor(renderer, (cursor_color & 0xFF000000) >> 24, (cursor_color & 0x00FF0000) >> 16, (cursor_color & 0x0000FF00) >> 8, (cursor_color & 0x000000FF));
		SDL_RenderFillRect(renderer, &d);
	}
	else if (timer.steps() > time_change*3)
	{
		timer.reset();
	}

	child_draw(renderer);
}

int GuiTextInput::update (  )
{
	for (auto &key: controls)
		key.second.update();
	
	GuiLabel::update();
	
	if (remakeTexture)
	{
		remakeTexture = false;
		str_to_surface(str, fontName);

		dim.w = get_texture_width();
		dim.h = get_texture_height();
	}

	if (controls["backspace"].get_state() == Key::PRESS && cursorStart > 0)
	{
		cursorStart--;
		if (cursorStart < 0)
			cursorStart = 0;
		cursor.x = get_pos().x + cursorStart * textsize;

		if (str.size())
			str.erase(str.begin() + cursorStart, str.begin() + cursorStart + 1);
		remakeTexture = true;
	}

	if (controls["delete"].get_state() == Key::PRESS && cursorStart < int(str.length()))
	{
		cursorStart++;
		if (cursorStart > int(str.length()))
			cursorStart = int(str.length());
		cursor.x = get_pos().x + cursorStart * textsize;

		if (str.size())
			str.erase(str.begin() + cursorStart, str.begin() + cursorStart + 1);
		remakeTexture = true;
	}

	if (controls["left"].get_state() == Key::PRESS)
	{
		cursorStart--;
		if (cursorStart < 0)
			cursorStart = 0;
		cursor.x = get_pos().x + cursorStart * textsize;
	}

	if (controls["right"].get_state() == Key::PRESS)
	{
		cursorStart++;
		if (cursorStart > int(str.length()))
			cursorStart = str.length();
		cursor.x = get_pos().x + cursorStart * textsize;
	}

	if (timer.steps() < time_change/2)
	{
		if (controls["backspace"].get_state() == Key::HOLD && cursorStart > 0)
		{
			cursorStart--;
			if (cursorStart < 0)
				cursorStart = 0;
			cursor.x = get_pos().x + cursorStart * textsize;
			
			if (str.size())
				str.erase(str.begin() + cursorStart, str.begin() + cursorStart + 1);
			remakeTexture = true;
		}
	}

	timer.update();

	return get_state();
}
