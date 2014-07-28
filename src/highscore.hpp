#ifndef HIGHSCORE_HPP
#define HIGHSCORE_HPP

#include "chora_engine/chora.hpp"
#include "chora_engine/gui/label.hpp"
#include "chora_engine/gui/writer.hpp"

struct SPlayerScore
{
	char name[11]; // quem fez a pontuação
	char score[11]; // a pontuação conseguida
	char level[11]; // a qual fase chegou
	
	SPlayerScore (  )
	{
		strcpy(name, "TUX");
		strcpy(score, "1000");
		strcpy(level, "01");
	}
	
	void clear (  )
	{
		SDL_memset(name, 0x0, sizeof(name));
		SDL_memset(score, 0x0, sizeof(score));
		SDL_memset(level, 0x0, sizeof(level));
	}
};

class CHighScore: public CWidget
{
	private:
		#if _WIN32 || _WIN64
			char path[FILENAME_MAX];
		#else
			char path[1024];
		#endif
	protected:
		SPlayerScore ps[10];
		CLabel * name;
		CLabel * score;
		CLabel * level;
		#if USE_SDL2
			SDL_Renderer * renderer;
		#endif
		bool can_set_widgets;
		int nscore; // novo score posição em ps + 1

	public:
		#ifndef USE_SDL2
			CHighScore (  );
		#else
			CHighScore ( SDL_Renderer * r );
		#endif
		
		~CHighScore (  );

		void delete_child (  );
		
		void set_widgets (  );
		
		bool save ( SPlayerScore p );
		
		bool read (  );
		
		// retorna 0 em caso de não high score
		// o retorno é a posição de cima para baixo
		int is_highscore ( int p );
};

#endif


