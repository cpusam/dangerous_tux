/*
	Programar!
*/

#ifndef SAVEGAME_HPP
#define SAVEGAME_HPP

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
		SDL_memset(score, 0x0, sizeof(score));
		SDL_memset(score_aux, 0x0, sizeof(score_aux));
	}
};

class CSaveGame
{
	private:
		SSaveData data;
		#if _WIN32 || _WIN64 || __MINGW32__
			char path[FILENAME_MAX];
		#else
			char path[1024];
		#endif

	public:
		CSaveGame ( )
		{
			#if _WIN32 || _WIN64 || __MINGW32__
				#ifndef PREFIX
					char p2[FILENAME_MAX];
					_getcwd(p2, sizeof(p2));
					sprintf(path, "%s\\DT_save", p2);
				#else
					sprintf(path, "%s\\dangeroustux\\DT_save", PREFIX);
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "DT_save");
				#else
					sprintf(path, "%s/DT_save", getenv("HOME"));
				#endif
			#endif
		}
		
		SSaveData get_data (  )
		{
			return data;
		}
		
		bool load (  )
		{
			FILE * file = fopen(path, "rb");
			
			if (!file)
			{
				cout << "CSaveGame: Não abriu " << path << endl;
				return false;
			}
			
			if (fread(&data, 1, sizeof(SSaveData), file) != sizeof(SSaveData))
			{
				cout << "CSaveGame: segunda não leu " << path << endl;
				fclose(file);
				return false;
			}
			
			fclose(file);
			return true;
		}
		
		bool save ( SSaveData s )
		{
			FILE * file = fopen(path, "wb+");
			
			if (!file)
				return false;

			data = s;
			if (fwrite(&data, 1, sizeof(SSaveData), file) != sizeof(SSaveData))
			{
				fclose(file);
				return false;
			}
			
			fclose(file);
			return true;
		}
};

#endif

