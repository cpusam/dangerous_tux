#include "textureid.hpp"

TextureID::TextureID(){
	path = "";
	name = "";
	texture = 0;
}

TextureID::TextureID(SDL_Texture *tex, std::string n){
	path = "";
	name = n;
	texture = tex;
	
	if (tex == 0)
	{
		throw "Erro: ao setar textura NULA";
	}
}

TextureID::TextureID(std::string path, SDL_Renderer *renderer){
	this->path = path;
	this->name = path;
	/*
	std::size_t begin = this->path.find_last_of("/");
	if(begin == std::string::npos)
		this->name = this->path; // aqui é sem '/'
	else if (begin <= path.length())
		this->name = this->path.substr(begin); // aqui é com barra
	else
	{
		std::string ret = "TextureID: erro ao ler path da textura" + std::string(SDL_GetError());
		throw ret.c_str();
	}
	*/

	texture = IMG_LoadTexture(renderer,static_cast<const char *>(path.c_str()));
	if(texture == 0){
		std::string ret = "[Texture ID] Erro ao carregar Textura: " + std::string(SDL_GetError());
		
		throw ret.c_str();
	}
}

TextureID::TextureID(std::string path, SDL_Renderer *renderer, SDL_Color colorKey){
	this->path = path;
	this->name = path;

	SDL_Surface * surf = IMG_Load(static_cast<const char *>(path.c_str()));
	if (surf == 0)
	{
		std::string ret = "[Texture ID] Erro ao carregar Surface: " + std::string(SDL_GetError());
		throw ret.c_str();
	}
	
	SDL_SetColorKey(surf,SDL_TRUE,SDL_MapRGBA(surf->format, colorKey.r,colorKey.g,colorKey.b,colorKey.a));
	
	texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	if(texture == 0){
		std::string ret = "[Texture ID] Erro ao carregar Textura: " + std::string(SDL_GetError());
		
		throw ret.c_str();
	}
}

TextureID::~TextureID(){
	//destroy();
}

void TextureID::destroy(){
	//if(texture)
	SDL_DestroyTexture(texture);
	texture = 0;
	path = "";
	name = "";
}
