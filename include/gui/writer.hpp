#ifndef WRITER_HPP
#define WRITER_HPP

enum ETextType
{
	SOLID_TEXT,
	UTF8_TEXT,
	UNICODE_TEXT
};

/*
	classe para escrever linha de texto com a SDL_ttf
*/
class CWriter
{
	private:
		int size; // tamanho da fonte
		string path; // caminho para a fonte
		TTF_Font * font;
		static CWriter * singleton;
		
		CWriter (  )
		{
			font = 0;
			size = 0;
		}
		
	public:
		static CWriter * instance (  )
		{
			if (!singleton)
				singleton = new CWriter();

			return singleton;
		}
		
		~CWriter (  )
		{
			if (singleton)
			{
				if (font)
					TTF_CloseFont(font);

				delete singleton;
			}
		}
		
		/*
		// removido
		void set_font ( TTF_Font * f )
		{
			if (!TTF_WasInit())
				throw "SDL_ttf não inicializada\n";

			font = f;
			if (!font)
				throw "Fonte inválida ou nula\n";
		}
		*/
		
		int set_font ( string p, int s )
		{
			if (!TTF_WasInit())
				throw "CWriter: SDL_ttf não inicializada\n";
			
			if (s > 0 && s == size && p == path)
				return -1;
			
			if (font)
				TTF_CloseFont(font);
			
			size = s;
			path = p;
			font = TTF_OpenFont(static_cast<const char *>(p.c_str()), s);
			if (!font)
			{
				cout << "CWriter: não conseguiu abrir font\n";
				return 0;
			}
			
			return 1;
		}
		
		TTF_Font * get_font (  )
		{
			return font;
		}
		
		bool resize_font ( int s )
		{
			if (!TTF_WasInit())
				throw "SDL_ttf não inicializada\n";

			if (!font || s <= 0)
				return false;
			
			TTF_CloseFont(font);
			font = TTF_OpenFont(path.c_str(), s);
			
			if (!font)
				throw "CWriter: não conseguiu redimensionar fonte\n";
			
			return true;
		}
		
		SDL_Surface * render_text ( string text, SDL_Color c, int type=SOLID_TEXT )
		{
			if (!TTF_WasInit())
				throw "SDL_ttf não inicializada\n";
			
			if (font == 0)
				throw "Nenhuma fonte usada\n";

			string str;
			vector <SDL_Surface *> tmp;
			SDL_Surface * surf = 0;
			int line = 0;
			
			for (int i(0); i < text.size(); i++)
			{
				if (text[i] == '\n' || i + 1 == text.size())
				{
					line++;
					if (i + 1 == text.size())
					{
						str.push_back(text[i]);
						str.push_back('\0');
					}
					
					switch (type)
					{
						case SOLID_TEXT:
							tmp.push_back(TTF_RenderText_Solid(font, str.c_str(), c));
							break;
						case UTF8_TEXT:
							tmp.push_back(TTF_RenderUTF8_Solid(font, str.c_str(), c));
							break;
						case UNICODE_TEXT:
							{
								Uint16 * s = (Uint16 *)malloc(sizeof(Uint16) * (str.length() + 1));
								for (int i(0); i < str.length(); i++)
									s[i] = str[i];
								s[str.length()] = '\0';
								tmp.push_back(TTF_RenderUNICODE_Solid(font, s, c));
								free(s);
							}
							break;

						default:
							tmp.push_back(TTF_RenderText_Solid(font, str.c_str(), c));
							break;
					}
					
					str.clear();
				}
				else
					str.push_back(text[i]);
			}
			
			if (line == 1)
			{
				switch (type)
				{
					case SOLID_TEXT:
						return TTF_RenderText_Solid(font, text.c_str(), c);
						
					case UTF8_TEXT:
						return TTF_RenderUTF8_Solid(font, text.c_str(), c);
						
					case UNICODE_TEXT:
						{
								Uint16 * s = (Uint16 *)malloc(sizeof(Uint16) * (str.length() + 1));
								for (int i(0); i < str.length(); i++)
									s[i] = str[i];
								s[str.length()] = '\0';
								SDL_Surface * srf = TTF_RenderUNICODE_Solid(font, s, c);
								free(s);
								return srf;
						}
						break;
						
					default:
						return TTF_RenderText_Solid(font, text.c_str(), c);
				}
			}

			int h = 0, w = 0;
			for (int i(0); i < tmp.size(); i++)
			{
				h += tmp[i]->h;
				if (tmp[i]->w > w)
					w = tmp[i]->w;
			}
			
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

			surf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                          rmask, gmask, bmask, amask);
			if(surf == NULL)
				throw SDL_GetError();
			
			for (int i(0); i < tmp.size(); i++)
			{
				SDL_Rect d;
				d.x = 0;
				d.y = tmp[i]->h * i;
				d.w = tmp[i]->w;
				d.h = tmp[i]->h;
				SDL_BlitSurface(tmp[i], NULL, surf, &d);
				SDL_FreeSurface(tmp[i]);
			}

			surf = optimize_surface_alpha(surf);

			return surf;
		}
};

CWriter * CWriter::singleton = 0;

#endif

