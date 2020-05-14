#include "tilemap.hpp"
#include <algorithm>

void TileMap::set_tilesize ( int ts )
{
	tilesize = ts;
}

void TileMap::set_pos ( Vect p )
{
	pos = p;
}

Vect TileMap::get_pos (  )
{
	return pos;
}

bool TileMap::set_tile ( int x, int y, int t )
{
	if (x < 0 || y < 0)
	{
		std::cout<<"TileMap: get_tile posições negativas!\n";
		return false;
	}

	x = (x) / tilesize;
	y = (y) / tilesize;

	int p = y*width + x;
	if (p >= int(tileset.size()))
		return false;
	else
		tileset[p] = t;

	return true;
}

void TileMap::set_tile ( int i, int t )
{
	if (i > -1 && i < int(tileset.size()))
	{
		tileset[i] = t;
		if (!has_tile(t))
			tiles.push_back(t);
	}
}
/*
int TileMap::get_tile ( int x, int y )
{
	if (x < 0 || y < 0)
	{
		static int p = 0;
		if (p < 100)
		{
			printf("CTilaMap: get_tile posições negativas x = %d, y = %d\n", x, y);
		}
		p++;
		//throw "TileMap: get_tile posições negativas!\n";
		return -1;
	}

	x = int((x - pos.x) / tilesize);
	y = int((y - pos.y) / tilesize);

	if (y >= height || x >= width)
		return -1;

	if (y * width + x < int(tileset.size()))
	{
		//if (x < width && y < height)
		return tileset[y * width + x];
	}

	return -1;
}
*/

Vect TileMap::get_tile_pos ( int i )
{
	Vect p;
	
	p.set((i % width)*tilesize, (i / width)*tilesize);
	
	if (i < 0 || i >= int(tileset.size()))
		p.zero();
	
	return p;
}

int TileMap::get_tile ( int i )
{
	if (i > -1 && i < int(tileset.size()))
		return tileset[i];

	return -1;
}

int TileMap::find_first_pos ( int t )
{
	for (unsigned int i = 0, end = tileset.size(); i < end; i++)
		if (t == tileset[i])
			return i;

	return -1; // sem posição ou não encontrado
}

int TileMap::get_tilesize (  )
{
	return tilesize;
}

int TileMap::get_width (  )
{
	return width;
}

int TileMap::get_height (  )
{
	return height;
}

SDL_Rect TileMap::get_dimension (  )
{
	return dimension;
}

bool TileMap::has_tile ( int t )
{
	return std::find(tiles.begin(), tiles.end(), t) != tiles.end();
}

bool TileMap::add_tile ( int t )
{
	if (has_tile(t))
		return false;

	tiles.push_back(t);
	return true;
}

void TileMap::remove_tile ( int t )
{
	for (std::vector <int>::iterator it = tiles.begin(), end = tiles.end(); it != end; it++)
		if (t == *it)
		{
			tiles.erase(it);
			break;
		}
}

/*
	Lẽ todo o vetor e se encontra um -1 soma uma linha
*/
int TileMap::read ( std::vector <int> & t )
{
	tileset.clear();
	width = height = 0;
	for (std::vector <int>::iterator i = t.begin(), end = t.end(); i < end; i++)
	{
		if (*i <= -1)
		{
			height++;
			if (width == 0)
				width = int(i - t.begin());
		}
		else
		{
			tileset.push_back(*i);
			if (!has_tile(*i))
				tiles.push_back(*i);
		}
	}

	if (t.back() != -1)
		height++;

	dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
	
	return 1;
}
/*
	cada linha de tileset está dividida com -1, ou seja,
	a cada tile sendo -1 é uma linha.
*/
int TileMap::read ( int * t, int size )
{
	tileset.clear();
	width = height = 0;
	for (int i(0); i < size; i++)
	{
		if (t[i] == -1)
		{
			height++;
			if (width == 0)
				width = i;
		}
		else if (t[i] >= ' ')
		{
			tileset.push_back(t[i]);
			if (!has_tile(t[i]))
				tiles.push_back(t[i]);
		}
	}

	if (t[size - 1] != -1)
		height++;

	dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
	return 1;
}

/*
	Procurar ler um mapa melhor que isso.
*/
int TileMap::read ( std::string filename )
{
	std::ifstream file(filename.c_str(), std::ifstream::in | std::ifstream::binary);
	if (!file)
		return 0;

	tileset.clear();
	width = height = 0;
	while (file)
	{
		char tile;
		file.get(tile);

		if (tile == '\n')
		{
			if (width == 0)
			{
				width = int(tileset.size());
			}
		}
		else if (file && tile >= ' ')
		{
			tileset.push_back(tile);
			if (!has_tile(tile))
				tiles.push_back(tile);
		}
	}
	height = tileset.size() / width;
	dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
	file.close();

	return 1;
}

int TileMap::read_from_xpm ( std::string filename )
{
	FILE * file = fopen(filename.c_str(), "r");

	if (!file)
	{
		printf("TileMap::Erro ao ler XPM\n");
		return 0;
	}

	int state = 0;
	int c = 0;
	std::string str;
	std::vector<std::string> tokens;
	for (; !feof(file);)
	{
		if (state == 0)
		{
			c = fgetc(file);
			if (feof(file))
				break;
			
			if (c == '\"')
				state = 1;
		}
		else if (state == 1)
		{
			str = "";
			do
			{
				c = fgetc(file);
				if (c != '\"')
					str.push_back(c);
			} while (c != '\"' && !feof(file));

			if (c == '\"')
			{
				str.push_back('\0');
				tokens.push_back(str);
				state = 0;
			}
			else
			{
				printf("TileMap::Erro ao fazer parsing no arquivo XPM\n");
				return 0;
			}
		}
	}
	fclose(file);

	#ifdef _DEBUG_
	int i = 0;
	for (auto & str: tokens)
	{
		printf("token[%d] = %s\n",i,str.c_str());
		i++;
	}
	#endif

	char b = 0, pattern = 0;
	int data[4];
	unsigned int rgb = 0;
	std::map<char, unsigned int> colors;
	if (sscanf(tokens[0].c_str(), "%d %d %d %d", &width, &height, &data[0], &data[1]) != 4)
	{
		printf("TileMap::sscanf falhou na copnversão da string tokens[0]\n");
		return 0;
	}

	for (int i = 1; i < 1 + data[0]; i++)
	{
		int value = sscanf(tokens[i].c_str(), "%c %c #%x", &b, &pattern, &rgb);
		if (value < 2)
		{
			printf("TileMap::sscanf falhou na copnversão da string tokens[%d] com value = %d\n",i, value);
			return 0;
		}

		if (pattern == 'c')
		{
			colors[b] = rgb;
			if (tokens[i].find("None") == std::string::npos)
				tiles.push_back(static_cast<int>(rgb));
			else
				tiles.push_back(static_cast<int>(b));
		}
	}

	for (size_t i = data[0] + 1, sizeH = tokens.size(); i < sizeH; i++)
	{
		for (size_t j = 0, sizeW = tokens[i].length(); j < sizeW; j++)
		{
			tileset.push_back(static_cast<int>(colors[tokens[i][j]]));
		}
	}

	return 1;
}

