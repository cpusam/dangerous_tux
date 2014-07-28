#include "save.hpp"

CSaveGame::CSaveGame ( std::string fn )
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

CSaveGame::~CSaveGame (  )
{
	
}

SSaveData CSaveGame::get_data (  )
{
	return data;
}

std::string CSaveGame::get_filename (  )
{
	return filename;
}

bool CSaveGame::was_loaded (  )
{
	return loaded;
}

void CSaveGame::erase_profile (  )
{
	if (remove(path) == -1)
		std::cout << "CSaveGame: não foi possível deletar " << filename << std::endl;
	else
		std::cout << "CSaveGame: profile " << filename << " deletado.\n";
}

bool CSaveGame::load (  )
{
	FILE * file = fopen(path, "rb");
	
	if (!file)
	{
		std::cout << "CSaveGame: Não abriu " << path << std::endl;
		loaded = false;
		return false;
	}
	
	if (fread(&data, 1, sizeof(SSaveData), file) != sizeof(SSaveData))
	{
		std::cout << "CSaveGame: segunda não leu " << path << std::endl;
		fclose(file);
		loaded = false;
		return false;
	}
	
	fclose(file);
	loaded = true;
	return true;
}

bool CSaveGame::save ( SSaveData s )
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


