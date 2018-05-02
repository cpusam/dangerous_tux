#ifndef HIGHSCORE_HPP
#define HIGHSCORE_HPP

#include "Chora.hpp"
#include "gui/label.hpp"
#include "gui/writer.hpp"

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
		memset(name, 0x0, sizeof(name));
		memset(score, 0x0, sizeof(score));
		memset(level, 0x0, sizeof(level));
	}
};

class CHighScore: public Widget
{
	private:
		#if _WIN32 || _WIN64
			char path[FILENAME_MAX];
		#else
			char path[1024];
		#endif
	protected:
		SPlayerScore ps[10];
		GuiLabel * name;
		GuiLabel * score;
		GuiLabel * level;
		SDL_Renderer * renderer;
		bool can_set_widgets;
		int nscore; // novo score posição em ps + 1

	public:
		CHighScore ( SDL_Renderer * r );
		
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


