#include "animation.hpp"
#include "collision.hpp"

void AnimationFrame::set_frame ( int d, SDL_Rect src )
{
	set_source(src);
	set_destiny((SDL_Rect){0,0,src.w,src.h});
	set_delay(d);
}

bool AnimationFrame::set_delay ( int d )
{
	if (d > -1)
		delay = d;

	return (d > -1);
}

int AnimationFrame::get_delay (  )
{
	return delay;
}

void AnimationFrame::set_source ( SDL_Rect s )
{
	source = s;
}

SDL_Rect AnimationFrame::get_source (  )
{
	return source;
}

void AnimationFrame::set_destiny ( SDL_Rect d )
{
	x = d.x;
	y = d.y;
	destiny = d;
}

SDL_Rect AnimationFrame::get_destiny (  )
{
	return destiny;
}


Vect AnimationFrame::get_orientation (  )
{
	return orientation;
}

float AnimationFrame::get_angle (  )
{
	return angle;
}

void AnimationFrame::set_angle ( float rad )
{
	angle = rad;
}

void AnimationFrame::rotate ( float a )
{
	angle += a;
	orientation.rotate(a);
}

void AnimationFrame::set_flip ( bool hor, bool ver )
{
	unsigned int f = static_cast<unsigned int>(flip);
	
	if (hor == true)
		f = f ^ static_cast<unsigned int>(SDL_FLIP_HORIZONTAL);
	
	if (ver == true)
		f = f ^ static_cast<unsigned int>(SDL_FLIP_VERTICAL);

	flip = static_cast<SDL_RendererFlip>(f);
}

SDL_RendererFlip AnimationFrame::get_flip (  )
{
	return flip;
}

void AnimationFrame::set_texture ( SDL_Texture * t )
{
	texture = t;
}

SDL_Texture * AnimationFrame::get_texture (  )
{
	return texture;
}

bool AnimationFrame::destroy (  )
{
	bool ret = false;

	if (texture)
	{
		//SDL_DestroyTexture(texture);
		texture = 0;
		ret = true;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////

void Animation::play (  )
{
	if (get_state() == STOPPED)
		set_state(START);
	else
		set_state(RUNNING);

	timer.start();
}

void Animation::pause (  )
{
	set_state(PAUSED);
	timer.pause();
}

void Animation::reset (  )
{
	index = 0;
	set_state(START);
	timer.reset();
}

void Animation::set_repeat ( bool r )
{
	repeat = r;
}

void Animation::set_delay ( int f, int d )
{
	if (f >= 0 && f < int(frames.size()))
		frames[f].set_delay(d);
}

int Animation::get_total_time (  )
{
	int total = 0;
	for (auto & frame: frames)
		total += frame.get_delay();
	return total;
}

// seta todos os frames para o mesmo delay
void Animation::set_frames_delay ( int d )
{
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].set_delay(d);
}

int Animation::get_frames_size (  )
{
	return frames.size();
}

void Animation::clear_frames ( bool destroy )
{
	frames.clear();
	if (destroy)
		destroy_textures();
	texture.clear();
}

Vect Animation::get_orientation (  )
{
	return orientation;
}

float Animation::get_angle (  )
{
	return angle;
}

void Animation::set_angle ( float rad )
{
	for (auto & frame: frames)
	{
		frame.set_angle(rad);
	}
}


void Animation::rotate ( float a )
{
	if (use_rot == false)
		return;

	angle += a;
	orientation.rotate(a);
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].rotate(a);
}

void Animation::set_use_rot ( bool u )
{
	use_rot = true;
}

bool Animation::get_use_rot (  )
{
	return use_rot;
}

void Animation::set_use_center ( bool u )
{
	use_center = true;
}

bool Animation::get_use_center (  )
{
	return use_center;
}

void Animation::set_center ( Vect center )
{
	this->center = center;
}

void Animation::flip ( SDL_RendererFlip f )
{
	bool hor, ver;
	
	hor = f & SDL_FLIP_HORIZONTAL;
	ver = f & SDL_FLIP_VERTICAL;
	
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].set_flip(hor, ver);
}

void Animation::flip ( bool hor, bool ver )
{
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].set_flip(hor, ver);
}

SDL_RendererFlip Animation::get_flip (  )
{
	return frames.at(0).get_flip();
}

void Animation::add_frame ( SDL_Texture * t, SDL_Rect const & src, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.set_source(src);
	f.set_destiny((SDL_Rect){0,0,src.w,src.h});
	f.set_delay(d);
	f.set_texture(t);

	frames.push_back(static_cast<AnimationFrame>(f));
}

void Animation::add_frame ( SDL_Texture * t, SDL_Rect const & src, SDL_Rect const & dst, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.set_source(src);
	f.set_destiny(dst);
	f.set_delay(d);
	f.set_texture(t);

	frames.push_back(f);
}

void Animation::add_frame ( SDL_Texture * t, AnimationFrame & f )
{
	index = 0;
	texture.push_back(t);
	f.set_texture(t);
	frames.push_back(f);
}

SDL_Texture * Animation::get_texture ( int i )
{
	if (texture.size() > 0 && i < int(texture.size()))
		return texture[i];

	return 0;
}

void Animation::destroy_textures (  )
{
	
	std::vector <SDL_Texture *> t;

	for (unsigned int i(0); i < texture.size(); i++)
	{
		SDL_Texture * aux = texture[i];
		int count = 0;
		for (unsigned int j(0); j < texture.size(); j++)
			if (j != i && aux == texture[j])
				count++;

		if (count == 0)
		{
			if (aux)
				t.push_back(aux);
		}
		else
		{
			count = 0;
			for (unsigned int j(0); j < t.size(); j++)
				if (t.at(j) == aux)
					count++;

			if (count > 0)
				if (aux)
					t.push_back(aux);
		}
	}

	for (unsigned int i(0); i < t.size(); i++)
		if (t.at(i))
			SDL_DestroyTexture(t.at(i));

	for (unsigned int i = 0; i < frames.size(); i++)
		frames[i].destroy();
	
	texture.clear();
	//t.clear();
}

bool Animation::has_texture ( SDL_Texture * t )
{
	if (!t)
		return false;

	for (size_t i(0), size = texture.size(); i < size; ++i)
		if (t == texture[i])
			return true;

	return false;
}

bool Animation::set_index ( int i )
{
	if (i >= 0 && i < int(frames.size()))
	{
		index = i;
		timer.reset();
		return true;
	}

	return false;
}

int Animation::get_index (  )
{
	return index;
}

void Animation::set_timer ( STimer t )
{
	timer = t;
}

STimer Animation::get_timer (  )
{
	return timer;
}

void Animation::set_name ( std::string n )
{
	name = n;
}

std::string Animation::get_name (  )
{
	return name;
}

std::vector<AnimationFrame> & Animation::get_frames (  )
{
	return frames;
}

AnimationFrame Animation::get_frame ( int i )
{
	if (i > 0 && i <= int(frames.size()))
		return frames[i];

	return AnimationFrame();
}

AnimationFrame Animation::get_curr_frame (  )
{
	return frames[index];
}

int Animation::draw ( SDL_Renderer * renderer, int x, int y )
{
	int ret = 0;
	static SDL_Rect dest, source;
	{
		dest = frames.at(index).get_destiny();
		source = frames.at(index).get_source();
	}

	dest.x = dest.x + x;
	dest.y = dest.y + y;

	if (use_center)
	{
		dest.x = dest.x - source.w / 2;
		dest.y = dest.y - source.h / 2;
	}

	if (texture.size() && texture.at(index))
	{
		if (use_rot == false)
		{
			ret = SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, 0, 0, frames[index].get_flip());
		}
		else
		{
			static SDL_Point center;
			if (use_center)
				center = {frames[index].get_source().w/2, frames[index].get_source().h/2};
			else if (use_rot)
			{
				center.x = this->center.x;
				center.y = this->center.y;
			}
			ret = SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}


int Animation::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y, int destW, int destH )
{
	int ret = 0;
	SDL_Rect dest, source;
	//if (get_state() == CHANGE_FRAME)
	{
		dest = frames.at(index).get_destiny();
		source = frames.at(index).get_source();
	}

	dest.w = destW;
	dest.h = destH;

	Vect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();
	//SDL_Rect view = cam->get_view();

	dest.x = dest.x + x;
	dest.y = dest.y + y;

	if (use_center)
	{
		dest.x = dest.x - dest.w / 2;
		dest.y = dest.y - dest.h / 2;
	}
	
	
	dest.x = (dest.x - pos.x) + dim.x;
	dest.y = (dest.y - pos.y) + dim.y;
	
	/*
	SDL_Rect rect = rectIntersect(dest,dim);
	dest = rect;
	*/

	if (texture.size() && texture.at(index))
	{
		//SDL_Color color = {255,255,0,128};
		//fill_rect(renderer, color, rect);
		// atualiza o viewport para desenhar nele
		//cam->updateViewport(renderer);
		if (use_rot == false)
			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, 0, 0, frames[index].get_flip());
		else
		{
			static SDL_Point center = {dest.w/2, dest.h/2};
			if (use_center == false && use_rot)
			{
				center.x = this->center.x;
				center.y = this->center.y;
			}

			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}

int Animation::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y )
{
	int ret = 0;
	SDL_Rect dest, source;
	dest = frames.at(index).get_destiny();
	source = frames.at(index).get_source();

	Vect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();
	//SDL_Rect view = cam->get_view();

	dest.x += x;
	dest.y += y;

	if (use_center)
	{
		dest.x = dest.x - dest.w / 2;
		dest.y = dest.y - dest.h / 2;
	}
	
	
	dest.x = (dest.x - pos.x) + dim.x;
	dest.y = (dest.y - pos.y) + dim.y;
	
	
	/*
	SDL_Rect rect = rectIntersect(dest,dim);
	dest = rect;
	*/

	if (texture.size() && texture.at(index))
	{
		//SDL_Color color = {255,255,0,128};
		//fill_rect(renderer, color, rect);
		// atualiza o viewport para desenhar nele
		//cam->updateViewport(renderer);
		if (use_rot == false)
			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, 0, 0, frames[index].get_flip());
		else
		{
			SDL_Point center = {dest.w/2, dest.h/2};
			if (use_center == false && use_rot)
			{
				center.x = this->center.x;
				center.y = this->center.y;
			}

			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}

int Animation::update (  )
{
	switch (get_state())
	{
		case START:
		case CHANGE_FRAME:
		case RUNNING:
		case FINISHED:
			if (frames.size() == 0)
			{
				printf("Animation: animação sem frames\n");
				return -1;
			}

			timer.update();
			if (timer.steps() >= frames[index].get_delay())
			{
				timer.reset();
				index++;
				if (index >= int(frames.size()))
				{
					if (repeat)
					{
						index = 0;
						set_state(FINISHED); // termina e repete a animação
						break;
					}
					else
					{
						index = int(frames.size() - 1);
						set_state(STOPPED); // terminou a animação e fica parado
						break;
					}
				}

				set_state(CHANGE_FRAME); // novo frame
				break;
			}

			set_state(RUNNING); // rodando
			break;

		default:
			break;
	}

	return get_state();
}

