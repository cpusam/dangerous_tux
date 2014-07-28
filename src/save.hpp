#ifndef SAVEGAME_HPP
#define SAVEGAME_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

struct SSaveData
{
	char curr_level[10]; // level onde se parou
	char lives[10]; // número de vidas do jogador
	char score[10]; // pontuação do jogador
	char score_aux[10]; // pontuação para ganhar uma vida
	
	SSaveData (  )
	{
		strcpy(curr_level, "-1");
		strcpy(lives, "10"); // vidas por padrão
		memset(score, 0x0, sizeof(score));
		memset(score_aux, 0x0, sizeof(score_aux));
	}
};

class CSaveGame
{
	private:
		SSaveData data;
		std::string filename;
		bool loaded;
		#if _WIN32 || _WIN64
			char path[FILENAME_MAX];
		#else
			char path[1024];
		#endif

	public:
		CSaveGame ( std::string fn );
		virtual ~CSaveGame (  );
		
		SSaveData get_data (  );
		
		std::string get_filename (  );
		
		bool was_loaded (  );
		
		void erase_profile (  );
		
		bool load (  );
		
		bool save ( SSaveData s );
};

#endif

