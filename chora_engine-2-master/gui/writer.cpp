#include "writer.hpp"

Writer * Writer::singleton = nullptr;

Writer::Writer (  )
{
	//font = 0;
	size = 0;
	renderer = 0;

	if (!TTF_WasInit())
		throw "Writer: SDL_ttf não inicializada\n";
}

Writer::~Writer (  )
{
	
}

Writer * Writer::instance()
{
	if (!singleton)
		singleton = new Writer();

	return singleton;
}

void Writer::free_fonts (  )
{
	bool closed = false;
	for (auto & it: fonts)
		if (it.second.font && it.first != "=>default")
		{
			if (it.second.font == fonts["=>default"].font)
				closed = true;
			TTF_CloseFont(it.second.font);
		}

	if (closed == false && fonts["=>default"].font)
		TTF_CloseFont(fonts["=>default"].font);

	fonts.clear();
}

int Writer::load_font ( std::string path, std::string name, int s )
{
	if (fonts.find(name) != fonts.end() && fonts[name].font != nullptr)
		TTF_CloseFont(fonts[name].font);

	Font font;
	font.size = s;
	font.path = path;
	font.name = name;
	font.font = TTF_OpenFont(static_cast<const char *>(path.c_str()), s);
	if (!font.font)
	{
		throw Exception("Writer::load_font não carregou uma fonta chmada "+name+" com path="+path);
	}
	
	fonts["=>default"] = font;
	fonts[name] = font;

	return 1;
}

void Writer::destroy (  )
{
	if (singleton)
	{
		singleton->free_fonts();
		delete singleton;
		singleton = 0;
	}
}

TTF_Font * Writer::get_font ( std::string name )
{
	if (fonts.find(name) != fonts.end())
		return fonts[name].font;

	return fonts["=>default"].font;
}

bool Writer::resize_font ( std::string name, int s )
{	
	Font font = fonts[name];
	if (fonts.size() == 0 || font.font == nullptr || s <= 0)
		return false;

	if (font.font)
	{
		std::vector<std::map<std::string, Font>::iterator> keys;
		for (auto & it: fonts)
			if (it.second.font == font.font)
			{
				keys.push_back(fonts.find(it.first));
			}
		
		//apaga todas as fontes que tem a mesma TTF_Font
		for (auto & key: keys)
			fonts.erase(key);
		
		TTF_CloseFont(font.font);
	}

	font.font = TTF_OpenFont(font.path.c_str(), s);
	font.size = s;
	
	if (!font.font)
		throw Exception("Writer: não conseguiu redimensionar fonte\n");

	fonts["=>default"] = font;
	fonts[name] = font;

	return true;
}

void Writer::set_renderer ( SDL_Renderer * r )
{
	renderer = r;
}

SDL_Renderer * Writer::get_renderer (  )
{
	return renderer;
}


SDL_Texture * Writer::render_text ( std::string name, std::string text, SDL_Color c, int type )
{	
	if (name == "")
	{
		if (fonts.find("=>default") == fonts.end())
			throw Exception("Writer::Sem primeira fonte definida");
		name = "=>default";
	}
	
	if (text == "")
	{
		printf("Writer: WARNING: sem texto visível.\n");
	}

	if (renderer == nullptr)
		throw Exception("Writer: nenhum renderer usado\n");

	SDL_Surface * surface = render_text_surface(name, text, c, type);
	if (!surface)
		throw Exception("Writer::erro ao criar surface auxiliar");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture)
	{
		char * e = new char[256];
		sprintf(e, "Writer: erro %s\n", SDL_GetError());
		throw Exception(e); // c++ é esquisito
	}

	return texture;
}

SDL_Surface * Writer::render_text_surface ( std::string name, std::string text, SDL_Color c, int type )
{
	if (name == "")
	{
		if (fonts.find("=>default") == fonts.end())
			throw Exception("Writer::Sem primeira fonte definida");
		name = "=>default";
	}
	
	TTF_Font * font = fonts[name].font;
	if (font == nullptr)
		throw Exception("Writer: Nenhuma "+name+" fonte usada\n");

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	if (text == "")
	{
		SDL_Surface * surface = SDL_CreateRGBSurface(0, 1,1,32, rmask, gmask, bmask, amask);
		printf("Writer[0]::%s\n", SDL_GetError());
		if (!surface)
			throw Exception("Writer: Erro ao criar surface vazia");
		
		return surface;
	}

	std::string str;
	std::vector <SDL_Surface *> tmp;
	SDL_Surface * surf = 0;
	int line = 0;

	for (unsigned int i(0); i < text.length(); i++)
	{
		if (text[i] == '\n' || i + 1 == text.length())
		{
			line++;
			if (i + 1 == text.length())
			{
				str.push_back(text[i]);
				str.push_back('\0');
			}

			SDL_Surface * surface = nullptr;
			switch (type)
			{
				case SOLID_TEXT:
					surface = TTF_RenderText_Solid(font, str.c_str(), c);
					printf("Writer[1]::%s\n", SDL_GetError());
					break;
				case UTF8_TEXT:
					surface = TTF_RenderUTF8_Solid(font, str.c_str(), c);
					printf("Writer[2]::%s\n", SDL_GetError());
					break;
			#ifndef EMSCRIPTEN
				case UNICODE_TEXT:
				{
					unsigned int length = str.length();
					Uint16 * s = (Uint16 *)malloc(sizeof(Uint16) * (length + 1));
					for (unsigned int i(0); i < length; i++)
						s[i] = str[i];
					s[length] = 0x0;
					surface = TTF_RenderUNICODE_Solid(font, s, c);
					free(s);
					printf("Writer[3]::%s\n", SDL_GetError());
				}
				break;
			#endif
				default:
					surface = TTF_RenderText_Solid(font, str.c_str(), c);
					printf("Writer[4]::%s\n", SDL_GetError());
					break;
			}

			if (surface == nullptr)
				throw Exception("Writer::Não foi possivel criar surface com texto \""+text+"\"");
			tmp.push_back(surface);
			str.clear();
		}
		else
			str.push_back(text[i]);
	}

	if (line <= 1)
	{
		if (tmp.size() == 1)
		{
			return tmp.back();
		}
		else
			throw Exception("Writer::Erro ao criar surface");
	}

	int h = 0, w = 0;
	for (unsigned int i(0); i < tmp.size(); i++)
	{
		h += tmp[i]->h;
		if (tmp[i]->w > w)
			w = tmp[i]->w;
	}

	surf = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
	printf("Writer[5]::%s\n", SDL_GetError());
	if(surf == nullptr)
	{
		printf("Error Surface: %s\n",SDL_GetError());
		throw Exception(SDL_GetError());
	}

	for (unsigned int i(0); i < tmp.size(); i++)
	{
		SDL_Rect d;
		d.x = 0;
		d.y = tmp[i]->h * i;
		d.w = tmp[i]->w;
		d.h = tmp[i]->h;
		SDL_BlitSurface(tmp[i], 0, surf, &d);
		SDL_FreeSurface(tmp[i]);
	}

	if (!surf)
		throw Exception("Writer: surface de retorno nula\n");

	return surf;
}

