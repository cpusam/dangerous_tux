#include "highscore.hpp"

#include <iostream>

#ifndef USE_SDL2
	CHighScore::CHighScore (  )
#else
	CHighScore::CHighScore ( SDL_Renderer * r )
#endif
{
	#if USE_SDL2
		renderer = r;
	#endif
	id = "high_score";
	nscore = 0;
	can_set_widgets = true;
	name = score = level = 0;
	
	#if _WIN32 || _WIN64
		char p2[FILENAME_MAX];
		#ifndef PREFIX
			sprintf(path, "%s\\DT_highscore", _getcwd(p2, sizeof(p2)));
		#else
			sprintf(path, "%s\\dageroustux\\DT_highscore", PREFIX);
		#endif
	#else
		#ifndef PREFIX
			sprintf(path, "DT_highscore");
		#else
			sprintf(path, "%s/DT_highscore", getenv("HOME"));
		#endif
	#endif
	if (read())
		std::cout << "CHighScore lido do arquivo\n";
	else
		set_widgets();
}

CHighScore::~CHighScore (  )
{
	delete_child();
}

void CHighScore::delete_child (  )
{
	CWidget * w;
	if (name)
	{
		w = name->get_child(0);
		for (int i(0); w; i++, w = name->get_child(i))
			delete w;
		name->clear_child();
		delete name;
		name = 0;
	}
	
	if (score)
	{
		w = score->get_child(0);
		for (int i(0); w; i++, w = score->get_child(i))
			delete w;
		score->clear_child();
		delete score;
		score = 0;
	}

	if (level)
	{
		w = level->get_child(0);
		for (int i(0); w; i++, w = level->get_child(i))
			delete w;
		level->clear_child();
		delete level;
		level = 0;
	}
	
	clear_child();
}

void CHighScore::set_widgets (  )
{
	if (!can_set_widgets)
		return;

	delete_child();
	#if _WIN32 || _WIN64
		char path[FILENAME_MAX];
		char p2[FILENAME_MAX];
		_getcwd(p2, sizeof(p2));
		#ifndef PREFIX
			sprintf(path, "%s\\fonts\\inhouseedition.ttf", p2);
		#else
			sprintf(path, "%s\\dangeroustux\\fonts\\inhouseedition.ttf", PREFIX);
		#endif
	#else
		char path[1024];
		#ifndef PREFIX
			sprintf(path, "./fonts/inhouseedition.ttf");
		#else
			sprintf(path, "%s/share/games/dangeroustux/fonts/inhouseedition.ttf", PREFIX);
		#endif
	#endif
	if (CWriter::instance()->set_font(path, 60) == 0)
		throw "CHighScore: não conseguiu abrir fonte\n";
	
	#if USE_SDL2
		CWriter::instance()->set_renderer(renderer);
	#endif
	
	name  = new CLabel("NAME",  (SDL_Color){255,255,255,255});
	score = new CLabel("SCORE", (SDL_Color){255,255,255,255});
	level = new CLabel("LEVEL", (SDL_Color){255,255,255,255});

	CLabel * cname;
	CLabel * cscore;
	CLabel * clevel;

	for (int i(0); i < 10; i++)
	{
		cname = 0;
		cscore = 0;
		clevel = 0;

		if (nscore - 1 != i)
		{
			cname  = new CLabel(ps[i].name,  (SDL_Color){255,0,0,255});
			cscore = new CLabel(ps[i].score, (SDL_Color){255,0,0,255});
			clevel = new CLabel(ps[i].level, (SDL_Color){255,0,0,255});
		}
		else
		{
			cname  = new CLabel(ps[i].name,  (SDL_Color){255,255,0,255});
			cscore = new CLabel(ps[i].score, (SDL_Color){255,255,0,255});
			clevel = new CLabel(ps[i].level, (SDL_Color){255,255,0,255});
		}
		
		name->add_child(cname);
		score->add_child(cscore);
		level->add_child(clevel);
		
		#ifndef USE_SDL2
			cname->set_rel_pos(SVect((name->get_surface()->w - cname->get_surface()->w)/2, i * cname->get_surface()->h + 60));
			cscore->set_rel_pos(SVect((score->get_surface()->w - cscore->get_surface()->w)/2, i * cscore->get_surface()->h + 60));
			clevel->set_rel_pos(SVect((level->get_surface()->w - clevel->get_surface()->w)/2, i * clevel->get_surface()->h + 60));
		#else
			cname->set_rel_pos(SVect((name->get_texture_width() - cname->get_texture_width())/2, i * cname->get_texture_height() + 60));
			cscore->set_rel_pos(SVect((score->get_texture_width() - cscore->get_texture_height())/2, i * cscore->get_texture_height() + 60));
			clevel->set_rel_pos(SVect((level->get_texture_width() - clevel->get_texture_width())/2, i * clevel->get_texture_height() + 60));
		#endif
	}
	
	name->set_pos(SVect(250,0));
	score->set_pos(SVect(name->get_pos().x + 180,0));
	#ifndef USE_SDL2
		level->set_pos(SVect(score->get_pos().x + score->get_surface()->w + 50,0));
	#else
		level->set_pos(SVect(score->get_pos().x + score->get_texture_width() + 50,0));
	#endif
	
	add_child(name);
	add_child(score);
	add_child(level);
	show_child();
	can_set_widgets = false;
}

bool CHighScore::save ( SPlayerScore p )
{
	int pscore[11];
	
	for (int i(0); i < 10; i++)
		sscanf(ps[i].score, "%d", &pscore[i]);
	sscanf(p.score, "%d", &pscore[10]);
	
	SPlayerScore aux[10];
	for (int i(0); i < 10; i++)
		if (pscore[10] >= pscore[i])
		{
			SDL_memcpy(aux, ps, sizeof(ps));
			for (int j(i); j < 9; j++)
				aux[j + 1] = ps[j];
			aux[i].clear();
			aux[i] = p;
			SDL_memcpy(ps, aux, sizeof(aux));

			FILE * file = fopen(path, "wb+");
			if (!file)
			{
				std::cout << "CHighScore: não foi possível abrir arquivo de score\n";
				return false;
			}
			if (fwrite(aux, 1, sizeof(aux), file) != sizeof(aux))
			{
				std::cout << "CHighScore: não foi possível salvar score\n";
				return false;
			}
			fclose(file);

			// faz releitura do high score
			read();
			nscore = i + 1;
			can_set_widgets = true;
			set_widgets();
			
			return true;
		}
	
	return false;
}

bool CHighScore::read (  )
{
	FILE * file = fopen(path, "rb");
	
	if (file == NULL)
		return false;

	SPlayerScore aux[10];
	if (fread(aux, 1, sizeof(aux), file) != sizeof(aux))
	{
		std::cout << "CHighScore: Erro ao ler arquivo DT_highscore\n";
		return false;
	}
	fclose(file);

	SDL_memcpy(ps, aux, sizeof(ps));
	set_widgets();

	return true;
}

// retorna 0 em caso de não high score
// o retorno é a posição de cima para baixo
int CHighScore::is_highscore ( int p )
{
	int score[11];
	
	for (int i(0); i < 10; i++)
		sscanf(ps[i].score, "%d", &score[i]);
	score[10] = p;
	
	for (int i(0); i < 10; i++)
		if (score[10] > score[i])
			return i + 1;
			
	return 0;
}


