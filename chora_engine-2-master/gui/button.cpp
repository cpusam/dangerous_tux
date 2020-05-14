#include "button.hpp"

GuiButton::GuiButton ( SDL_Rect d )
{
	label = nullptr;
	callback = nullptr;
	this->texture = nullptr;
	color1 = (SDL_Color){0xFF, 0xFF, 0x00, 0xFF};
	color2 = (SDL_Color){0x00, 0xFF, 0xFF, 0xFF};
	color3 = (SDL_Color){0xFF, 0x00, 0x00, 0xFF};
	pos.x = d.x, pos.y = d.y;
	// dimensão padrão
	dim = d;
	set_state(State::NORMAL);
	run_release = false;
	texture = nullptr;
}

GuiButton::GuiButton ( SDL_Rect d, std::string str, std::string fontName, SDL_Rect * src, SDL_Texture * texture )
{
	label = nullptr;
	callback = nullptr;
	color1 = (SDL_Color){0xFF, 0xFF, 0x00, 0xFF};
	color2 = (SDL_Color){0x00, 0xFF, 0xFF, 0xFF};
	color3 = (SDL_Color){0xFF, 0x00, 0x00, 0xFF};
	pos.x = d.x, pos.y = d.y;
	// dimensão padrão
	dim = d;
	set_label(new GuiLabel(str, (SDL_Color){0,0,0,255}, fontName)); // por padrão na cor preta
	set_state(State::NORMAL);
	this->texture = nullptr;
	set_texture(texture);
	set_sources(src);
	run_release = false;
}

GuiButton::~GuiButton (  )
{
	//widget::destroy trata de dar conta de label, que foi adicionada com add_child
}

void GuiButton::press (  )
{
	if (get_state() != State::PRESSED)
		set_state(State::PRESSED);
}

void GuiButton::select (  )
{
	if (get_state() != State::SELECTED)
		set_state(State::SELECTED);
}

void GuiButton::release (  )
{
	if (get_state() != State::RELEASED)
	{
		set_state(State::RELEASED);
		run_release = false;
	}
}

void GuiButton::set_texture ( SDL_Texture * texture )
{
	if (this->texture)
	{
		SDL_DestroyTexture(this->texture);
		this->texture = nullptr;
	}
	this->texture = texture;
}

void GuiButton::set_sources ( SDL_Rect src[3] )
{
	if (src)
	{
		this->src[0] = src[0];
		this->src[1] = src[1];
		this->src[2] = src[2];
	}
	else
	{
		this->src[0] = (SDL_Rect){0,0,1,1};
		this->src[1] = this->src[0];
		this->src[2] = this->src[0];
	}
}

SDL_Texture * GuiButton::get_texture (  )
{
	return texture;
}

GuiLabel * GuiButton::get_label (  )
{
	return label;
}

void GuiButton::set_callback ( void (* c) ( Widget * b ) )
{
	callback = c;
}

void GuiButton::set_dim ( SDL_Rect d )
{
	dim = d;
	pos.x = d.x;
	pos.y = d.y;

	if (!label)
		return;

	Vect p;
	SDL_Rect dst;
	dst = label->get_dim();
	// posição do label relativo ao botão
	p.x = (dst.w - d.w)/2.0f;
	p.y = (dst.h - d.h)/2.0f;
	label->set_rel_pos(p);
}

void GuiButton::set_label ( GuiLabel * l )
{
	if (!l)
		return;

	if (label && label != l)
	{
		rem_child(label);
		delete label;
		label = nullptr;
		throw 1;
	}


	label = l;
	add_child(label);

	Vect p;
	SDL_Rect d;
	int w, h;
	d = label->get_dim();
	w = d.w, d.w += 10;
	h = d.h, d.h += 5;
	dim.w = d.w, dim.h = d.h; // tamanho do botão
	// posição do label relativo ao botão
	p.x = (d.w - w)/2.0f;
	p.y = (d.h - h)/2.0f;
	label->set_rel_pos(p);
}

void GuiButton::input ( SDL_Event & event )
{
	if (!visible)
		return;

	Vect p;
	switch (event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				p.set(event.button.x, event.button.y);
				if (get_state() == State::SELECTED || get_state() == State::NORMAL)
				{
					if (event.button.state == SDL_PRESSED && pointbox(p, get_dim()))
						set_state(State::PRESSED);
				}
				else if (get_state() == State::RELEASED)
				{
					if (!run_release && pointbox(p, get_dim()))
					{
						set_state(State::PRESSED);
					}	
				}
			}
			break;
			
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				p.set(event.button.x, event.button.y);
				if (get_state() == State::PRESSED)
					if (event.button.state == SDL_RELEASED)
					{
						run_release = true;
						set_state(State::RELEASED);
					}
			}
			break;
		
		case SDL_MOUSEMOTION:
			p.x = event.motion.x;
			p.y = event.motion.y;
			if (pointbox(p, dim))
			{
				if (get_state() == State::NORMAL || get_state() == State::RELEASED)
					set_state(State::SELECTED);
			}
			else
			{
				set_state(State::NORMAL);
			}
			break;
		default:
			break;
	}

	child_input(event);
}

int GuiButton::update (  )
{
	int x, y;
	Vect p;
	SDL_GetMouseState(&x, &y);
	p.x = x, p.y = y;

	switch (get_state())
	{
		case State::NORMAL: // normal
			break;
		case State::SELECTED: // selecionado
			break;
		case State::PRESSED: // pressionado
			break;
		case State::RELEASED: // solto depois de pressionado
		{
			if (get_state() == run_release)
			{
				if (callback)
					callback(this);
				run_release = false;
			}
			if (pointbox(p, get_dim()))
				set_state(State::SELECTED);
			else
				set_state(State::NORMAL);
			break;
		}
		default:
			break;
	}

	child_update();
	
	return get_state();
}

void GuiButton::draw ( SDL_Renderer * renderer )
{
	if (!visible)
		return;
	
	if (!texture)
	{
		SDL_Rect rect = dim;
		switch (get_state())
		{
			case State::NORMAL:
				SDL_SetRenderDrawColor(renderer, color1.r, color1.b, color1.g, color1.a);
				break;
			case State::SELECTED:
				SDL_SetRenderDrawColor(renderer, color2.r, color2.b, color2.g, color2.a);
				break;
			case State::PRESSED:
				SDL_SetRenderDrawColor(renderer, color3.r, color3.b, color3.g, color3.a);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, color1.r, color1.b, color1.g, color1.a);
				break;
		}

		SDL_RenderFillRect(renderer, &rect);
		child_draw(renderer);
		return;
	}
	
	SDL_Rect d = dim;
	switch (get_state())
	{
		case State::NORMAL:
			if (SDL_RenderCopy(renderer,texture, &src[0], &d) == -1)
				throw SDL_GetError();
			break;
		case State::SELECTED:
			if (SDL_RenderCopy(renderer,texture, &src[1], &d) == -1)
				throw SDL_GetError();
			break;
		case State::PRESSED:
			if (SDL_RenderCopy(renderer,texture, &src[2], &d) == -1)
				throw SDL_GetError();
			break;
		default:
			if (SDL_RenderCopy(renderer,texture, &src[0], &d) == -1)
				throw SDL_GetError();
			break;
	}

	child_draw(renderer);
}


