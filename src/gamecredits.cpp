#include "gamecredits.hpp"

CGameCredits::CGameCredits ( SDL_Renderer * r )
{
	SDL_Surface * aux;
	
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX], bg_path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
	#else
		char path[1024], bg_path[1024];
	#endif

	#if _WIN32 || _WIN64
		#ifndef PREFIX
			sprintf(path, "%s\\fonts\\inhouseedition.ttf", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\fonts\\inhouseedition.ttf", PREFIX);
		#endif
	#else
		#ifndef PREFIX
			sprintf(path, "./fonts/inhouseedition.ttf");
		#else
			sprintf(path, "%s/share/games/dangeroustux/fonts/inhouseedition.ttf", PREFIX);
		#endif
	#endif
	
	if (!Writer::instance()->load_font(path, path, 100))
		throw "CGameCredits: não foi possível carregar font\n";

	Writer::instance()->set_renderer(r);
	
	
	char s[5][32] =
	{
		{71,82,65,80,72,73,67,83},
		{71,85,83,84,65,86,79,32,77,69,68,69,73,82,79,83},
		{80,82,79,71,82,65,77,77,73,78,71},
		{83,65,77,85,69,76,32,76,69,79,78,65,82,68,79},
		{84,72,73,65,71,79,32,72,85,80,78,69,82},
	};
	
	
	GuiLabel * g = new GuiLabel(s[0], (SDL_Color){255,255,255,0});
	widget.add_child(g);
	GuiLabel * gg = new GuiLabel(s[1], (SDL_Color){255,255,0,0});
	widget.add_child(gg);
	GuiLabel * p = new GuiLabel(s[2], (SDL_Color){255,255,255,0});
	widget.add_child(p);
	GuiLabel * ps = new GuiLabel(s[3], (SDL_Color){255,255,0,0});
	widget.add_child(ps);
	GuiLabel * t = new GuiLabel(s[4], (SDL_Color){255,255,0,0});
	widget.add_child(t);

	widget.set_pos(Vect(960/2,624/2));

	int h = g->get_texture_height() + gg->get_texture_height() + p->get_texture_height() + ps->get_texture_height();
	g->set_rel_pos(Vect(-(g->get_texture_width()/2), h));
	gg->set_rel_pos(Vect(-(gg->get_texture_width()/2), g->get_texture_height() + g->get_rel_pos().y));
	p->set_rel_pos(Vect(-(p->get_texture_width()/2), gg->get_texture_height() + gg->get_rel_pos().y));
	ps->set_rel_pos(Vect(-(ps->get_texture_width()/2), p->get_texture_height() + p->get_rel_pos().y));
	t->set_rel_pos(Vect(-(t->get_texture_width()/2), ps->get_texture_height() + ps->get_rel_pos().y));
	
	
	#if _WIN32 || _WIN64
		#ifndef PREFIX
				sprintf(path, "%s\\images\\tux_walk.png", p2);
			#else
				sprintf(path, "%s\\dangeroustux\\images\\tux_walk.png", PREFIX);
			#endif
		#else
			#ifndef PREFIX
				sprintf(path, "./images/tux_walk.png");
			#else
				sprintf(path, "%s/share/games/dangeroustux/images/tux_walk.png", PREFIX);
		#endif
	#endif
	
	#if _WIN32 || _WIN64
			#ifndef PREFIX
					sprintf(bg_path, "%s\\images\\credits_BG.png", p2);
				#else
					sprintf(bg_path, "%s\\dangeroustux\\images\\credits_BG.png", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(bg_path, "./images/credits_BG.png");
				#else
					sprintf(bg_path, "%s/share/games/dangeroustux/images/credits_BG.png", PREFIX);
			#endif
		#endif

	anim.add_frame(NULL, (SDL_Rect){0,0,0,0}, 15000);

	SDL_Texture * texture = IMG_LoadTexture(r, path);
	if (!texture)
		throw "CGameCredits: não foi possivel carregar tux_walk.png\n";

	tux_anim.add_frame(texture, (SDL_Rect){0,    0,214,234}, 200);
	tux_anim.add_frame(texture, (SDL_Rect){0,  234,214,234}, 200); // meio
	tux_anim.add_frame(texture, (SDL_Rect){0,2*234,214,234}, 200);
	tux_anim.add_frame(texture, (SDL_Rect){0,  234,214,234}, 200); // meio
	//tux_pos.x = widget.get_pos().x - texture_width(texture)/2;
	tux_pos.x = (960 - texture_width(texture))/2;

	if (!bg.set_texture(IMG_LoadTexture(r, bg_path)))
		throw "CGameCredits: não foi possível carregar credits_BG.png\n";
	//widget.set_pos(Vect(960/2, 358/2));
	tux_pos.y = 358;

	cam = new Camera((SDL_Rect){0,0,texture_width(bg.get_texture()),texture_height(bg.get_texture())}, (SDL_Rect){0,0,2000*texture_width(bg.get_texture()),texture_height(bg.get_texture())});
	set_state(ACTIVE_CREDITS);
}

CGameCredits::~CGameCredits (  )
{

	Widget * w = widget.get_child(0);
	for (int i = 0; w; i++, w = widget.get_child(i))
		delete w;


	delete cam;
		tux_anim.destroy_textures();
}


void CGameCredits::draw ( SDL_Renderer * renderer )
{
	SDL_SetRenderDrawColor(renderer, 0x00,0xc6,0xff,0xFF);
	SDL_RenderFillRect(renderer, NULL);

	bg.draw_hor(renderer, cam);
	tux_anim.draw(renderer, tux_pos.x, tux_pos.y);

	widget.draw(renderer);
}

void CGameCredits::reset (  )
{
	bg_pos.zero();
	cam->set_position(Vect());
	anim.reset();
	tux_anim.reset();
	set_state(ACTIVE_CREDITS);
}

int CGameCredits::update (  )
{
	bg_pos.x += 5.50f;
	cam->set_position(bg_pos);
	
	auto children = widget.get_children();
	for (auto child : children){
		auto pos = child->get_pos();
		if (pos.y < -1000) break;
		pos.y -= 2.5f;
		child->set_pos(pos);
	}
	widget.child_update();
	widget.update();
	tux_anim.update();
	if (anim.update() == 3)
		set_state(INACTIVE_CREDITS);
	
	return get_state();
}


