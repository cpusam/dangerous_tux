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
		string filename;
		bool loaded;
		#if _WIN32 || _WIN64
			char path[FILENAME_MAX];
		#else
			char path[1024];
		#endif

	public:
		CSaveGame ( string fn )
		{
			filename = fn;
			#if _WIN32 || _WIN64
				#ifndef PREFIX
					char p2[FILENAME_MAX];
					_getcwd(p2, sizeof(p2));
					sprintf(path, "%s\\%s", p2, filename.c_str());
				#else
					sprintf(path, "%s\\dangeroustux\\%s", PREFIX, filename.c_str());
				#endif
			#else
				#ifndef PREFIX
					sprintf(path, "%s", filename.c_str());
				#else
					sprintf(path, "%s/%s", getenv("HOME"), filename.c_str());
				#endif
			#endif
		}
		
		SSaveData get_data (  )
		{
			return data;
		}
		
		string get_filename (  )
		{
			return filename;
		}
		
		bool was_loaded (  )
		{
			return loaded;
		}
		
		void erase_profile (  )
		{
			if (remove(path) == -1)
				cout << "CSaveGame: não foi possível deletar " << filename << endl;
			else
				cout << "CSaveGame: profile " << filename << " deletado.\n";
		}
		
		bool load (  )
		{
			FILE * file = fopen(path, "rb");
			
			if (!file)
			{
				cout << "CSaveGame: Não abriu " << path << endl;
				loaded = false;
				return false;
			}
			
			if (fread(&data, 1, sizeof(SSaveData), file) != sizeof(SSaveData))
			{
				cout << "CSaveGame: segunda não leu " << path << endl;
				fclose(file);
				loaded = false;
				return false;
			}
			
			fclose(file);
			loaded = true;
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

