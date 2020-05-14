/*
	-Programa para criar fontes para os programa que usam a chora engine java
	02/12/2015 às 20:03
	./fonter font=20db.otf size=15 dim=512 color=0/0/0/255 text=sacii fileName=fontOut.txt
*/

#include "Chora.hpp"
#include <cstring>
#include <string>
#include "SDL_SavePNG/savepng.h"

int main ( int argc, char ** argv )
{
	if (argc < 5)
	{
		printf("Usage: %s font=\"file.ttf\" size=N dim=N color=R/G/B/A text=AbCdE...|ascii fileName=fileOut.txt \n", argv[0]);
		printf("dim deve ser em potencia de 2\n");
		return 1;
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	if (TTF_Init() < 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_Window * window = SDL_CreateWindow("Fonter", 0,0, 512,512, 0);
	if (!window)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_Renderer * renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	char font[256];
	int size = 50;
	char text[512] = "\0";
	int dim = 512;
	char name[256];
	memset(name, 0, sizeof(name));
	SDL_Color color = {255,255,255,255};
	int count = 0;
	for (int i = 0; i < argc; i++)
	{
		if (strstr(argv[i], "font="))
		{
			sscanf(argv[i],"font=%s",font);
			count++;
		}
		
		if (strstr(argv[i],"size="))
		{
			sscanf(argv[i],"size=%d",&size);
			count++;
		}
		
		if (strstr(argv[i],"text="))
		{
			sscanf(argv[i],"text=%s",text);
			count++;
		}
		
		if (strstr(argv[i],"color="))
		{
			sscanf(argv[i],"color=%hhu/%hhu/%hhu/%hhu", &color.r, &color.g,&color.b,&color.a);
			count++;
		}
		
		if (strstr(argv[i],"dim="))
		{
			sscanf(argv[i],"dim=%d", &dim);
			count++;
		}
		
		if (strstr(argv[i],"fileName="))
		{
			sscanf(argv[i],"fileName=%s", name);
			count++;
		}
	}
	
	if (strstr(text, "ascii"))
	{
		memset(text, 0x0,sizeof(text));
		for (int i = 0; i < 256; i++)
			text[i] = (char)(i + ' ');
	}
	
	if (count < 6)
	{
		printf("faltou parametros do fonter\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
	
		SDL_Quit();
		return 1;
	}
	
	for (int i = 0; text[i] != '\0'; i++)
		if (text[i] == '\\' && text[i + 1] == 'n')
		{
			text[i] = '\n';
			for (i++; text[i]; i++)
				text[i] = text[i + 1];
		}
	
	Writer::instance()->set_renderer(renderer);
	try
	{
		Writer::instance()->load_font("=>default", font, size);
	}
	catch (const char * e)
	{
		printf("Error: %s\n", e);
	}
	
	SDL_Surface * bmp;
	try
	{
		bmp = Writer::instance()->render_text_surface("=>default", std::string(text),color);
	}
	catch (const char * e)
	{
		printf("Error: %s\n", e);
	}
	
	SDL_Surface * surf;
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

	surf = SDL_CreateRGBSurface(SDL_SWSURFACE, dim, dim, 32, rmask, gmask, bmask, amask);
	std::vector <SDL_Surface *> chars;

	SDL_Surface * tmp = 0;
	for (int i = 0; text[i]; i++)
	{
		char str[2] = {text[i], '\0'};
		chars.push_back(Writer::instance()->render_text_surface("=>default", str, color));
	}
	
	

	FILE * file = fopen(name, "w+");
	if (!file)
	{
		printf("Não conseguiu abrir arquivo de saida\n");
		return 1;
	}
	
	
	SDL_Rect d = {0,0,0,0};
	for (int i = 0, w = 0, h = 0; i < chars.size(); i++)
	{
		d.x = w;
		d.y = h;
		d.w = chars[i]->w;
		d.h = chars[i]->h;
		
		if (d.x + d.w > dim)
		{
			d.x = 0;
			d.y = h + d.h;
			w = d.w;
			h += d.h;
		}
		else
		{
			w += chars[i]->w;
			if (w > dim)
			{
				w = 0;
				h += chars[i]->h;
			}
		}
		
		if (h > dim)
		{
			h = 0;
			printf("Erro no char %d: tamanho dim muito pequeno para tanto caracteres\n", i);
			continue;
		}
		
		SDL_BlitSurface(chars[i], NULL, surf, &d);
		fprintf(file, "%c %d %d %d %d\n", text[i], d.x, d.y, d.w, d.h);
	}
	fclose(file);
	
	sprintf(font, "font%d.png", size);
	SDL_SavePNG(surf, font);
	
	sprintf(font, "font%d.bmp", size);
	#if SAVEBMP
	SDL_SaveBMP(bmp, font);
	#endif
	
	
	
	SDL_FreeSurface(bmp);
	SDL_FreeSurface(surf);
	for (int i = 0; i < chars.size(); i++)
		SDL_FreeSurface(chars[i]);
	chars.clear();
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	SDL_Quit();
	
	return 0;
	
}


