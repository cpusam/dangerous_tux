#include "Entity.hpp"
#include "Exception.hpp"
#include "SDL_gfx/SDL_framerate.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

EntityID Entity::countID = 1;

Entity::Entity()
{
	layer = 0;
	id = countID;
	countID++;
	std::cout<<"ID="<<id<<std::endl;
	dir = NONE_DIR;
	visible = true;

	ground = false;
	level = nullptr;
	collPoints = 3;
	topLadderTile = -1;
	topTileSize = 10;//mesmo valor que maxVel;
	texture = nullptr;
	currAnim = nullptr;
	name = "";
	countPath = 0;
	
	//coloca a velocidade máxima... ao máximo
	maxVel.set(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	//máximo de desaceleração, pará bruscamente quando não movendo
	damping.set(0.0,0.0);
	//
	setCollRect((SDL_Rect){0,0,3,3}, collPoints);
}

Entity::~Entity()
{

}

int Entity::getLayer (  )
{
	return layer;
}

void Entity::setLayer ( int layer )
{
	this->layer = layer;
}

void Entity::setName ( std::string n )
{
	name = n;
}

std::string Entity::getName (  )
{
	return name;
}

void Entity::setGroup ( std::string g )
{
	group = g;
}

std::string Entity::getGroup (  )
{
	return group;
}

EntityID Entity::getId (  )
{
	return id;
}

void Entity::show ( bool s )
{
	visible = s;
}

bool Entity::isVisible (  )
{
	return visible;
}

void Entity::setDir ( Direction d )
{
	dir = d;
}

Direction Entity::getDir (  )
{
	return dir;
}

Vect Entity::getMinVel (  )
{
	return minVel;
}

Vect Entity::getMaxVel (  )
{
	return maxVel;
}

void Entity::setMinVel ( Vect minVel )
{
	this->minVel = minVel;
}

void Entity::setMaxVel ( Vect maxVel )
{
	this->maxVel = maxVel;
}

Vect Entity::getDamping (  )
{
	return damping;
}

void Entity::setDamping ( Vect d )
{
	damping = d;
}

void Entity::addAnim ( Animation & anim, std::string name )
{
	this->anim[name] = anim;
}

Animation * Entity::getCurrAnim (  )
{
	return currAnim;
}

std::string Entity::getAnimName ( std::string animName )
{
	return animName;
}

bool Entity::setCurrAnim ( std::string animName )
{
	std::map<std::string, Animation>::const_iterator it = anim.find(animName);

	if (it != anim.end())
	{
		currAnim = &anim[animName];
		return true;
	}

	return false;
}

void Entity::changeAnim ( std::string animName, bool reset )
{
	if (anim.size() > 0)
	{
		animName = getAnimName(animName);
		if (anim.find(animName) != anim.end())
		{
			currAnim = &anim[animName];
		}
		else
		{
			currAnim = nullptr;
			std::cout<<"Entity->"<<getName()<<"::Não encontrou uma animação chamada "<<animName<<std::endl;
		}
	}

	if (reset && currAnim)
		currAnim->reset();
}

void Entity::updateAnim (  )
{
	if (currAnim)
		currAnim->update();
}

Vect Entity::getPosition (  )
{
	return pos;
}

void Entity::setPosition ( Vect p )
{
	pos = p;
}

void Entity::setTopLadderTile ( int t )
{
	topLadderTile = t;
}

int Entity::getTopLadderTile (  )
{
	return topLadderTile;
}

void Entity::changeDir ( Direction d )
{
	bool changeH = true, changeV = true;

	if (dir == d)
		return;


	changeH = (d & RIGHT_DIR)||(d & LEFT_DIR);
	changeV = (d & UP_DIR)||(d & DOWN_DIR);

	for (std::map<std::string,Animation>::iterator it = anim.begin(); it != anim.end(); it++)
		it->second.flip(changeH, changeV);

	dir = d;
}

bool Entity::receive ( Entity * sender, std::string mesg )
{
	return false;
}

TileMap * Entity::getLevel (  )
{
	return level;
}

void Entity::setLevel ( TileMap * level )
{
	this->level = level;
}

void Entity::setCollPos ( Vect p )
{
	pos.x = p.x - collRect.x;
	pos.y = p.y - collRect.y;
}

Vect Entity::getCollPos (  )
{
	return Vect(pos.x + collRect.x, pos.y + collRect.y);
}

Vect Entity::getCollCenter (  )
{
	return Vect(int(pos.x)+collRect.x+collRect.w/2,int(pos.y)+collRect.y+collRect.h/2);
}

SDL_Rect Entity::getCollRect ( RelativePosition relative )
{
	if (relative == RELATIVE_WORLD)
		return (SDL_Rect){int(pos.x) + collRect.x, int(pos.y) + collRect.y, collRect.w,collRect.h};
	
	return collRect;
}

void Entity::setCollRect ( SDL_Rect rect, int numPoints )
{
	collRect = rect;
	if (numPoints > 2)
		setSides(rect, numPoints);
}

SDL_Rect Entity::getView (  )
{
	return (SDL_Rect){int(pos.x) + view.x, int(pos.y) + view.y, view.w,view.h};
}

bool Entity::isDir ( Direction d )
{
	return d & dir;
}

bool Entity::isSolidSide ( std::string side, int i )
{
	std::vector<Vect> vet;
	if (side == "up")
		vet = upSide;
	else if (side == "down")
		vet = downSide;
	else if (side == "right")
		vet = rightSide;
	else if (side == "left")
		vet = leftSide;
	else
	{
		std::cout<<"Error "<<side<<" lado não identificado\n";
		return false;
	}

	if (i >= 0)
	{
		if (i < int(vet.size()))
		{
			Vect p = vet.at(i);

			if (side == "up")
				p.y -= 1;
			else if (side == "down")
				p.y += 1;
			else if (side == "right")
				p.x += 1;
			else if (side == "left")
				p.x -= 1;

			return isSolid(p + pos);
		}

		return false;
	}

	for (auto & v: vet)
	{
		if (side == "up")
			v.y -= 1;
		else if (side == "down")
			v.y += 1;
		else if (side == "right")
			v.x += 1;
		else if (side == "left")
			v.x -= 1;

		if (isSolid(Vect::add(v, pos)))
			return true;
	}

	return false;
}


bool Entity::isSolidSide ( std::string side, SDL_Rect other, int i )
{
	std::vector<Vect> vet;
	if (side == "up")
		vet = upSide;
	else if (side == "down")
		vet = downSide;
	else if (side == "right")
		vet = rightSide;
	else if (side == "left")
		vet = leftSide;
	else
	{
		std::cout<<"Error "<<side<<" lado não identificado\n";
		return false;
	}

	if (i >= 0)
	{
		if (i < int(vet.size()))
		{
			Vect p = vet.at(i);

			if (side == "up")
				p.y -= 1;
			else if (side == "down")
				p.y += 1;
			else if (side == "right")
				p.x += 1;
			else if (side == "left")
				p.x -= 1;

			return pointbox(Vect::add(p, pos), other);
		}

		return false;
	}

	for (auto & v: vet)
	{
		if (side == "up")
			v.y -= 1;
		else if (side == "down")
			v.y += 1;
		else if (side == "right")
			v.x += 1;
		else if (side == "left")
			v.x -= 1;

		if (pointbox(Vect::add(v, pos), other))
			return true;
	}

	return false;
}

void Entity::setSolids ( std::vector<int> solids )
{
	solid = solids;
}

void Entity::setOneWayUp ( std::vector<int> oneWayUp )
{
	upSolid = oneWayUp;
}

void Entity::setSlopeUp ( std::vector<int> slopeUp, std::map<int, std::vector<Vect> > angles )
{
	upSolidSlope = slopeUp;
	upSolidSlopeAngles = angles;
}

std::vector<int> Entity::getSlopeUpTiles (  )
{
	return upSolidSlope;
}

std::map<int, std::vector<Vect> > Entity::getSlopeUpAngles (  )
{
	return upSolidSlopeAngles;
}

std::vector<int> Entity::getSolids (  )
{
	return solid;
}

std::vector<int> Entity::getOneWayUp (  )
{
	return upSolid;
}

void Entity::addSolid ( int s )
{
	for (auto i: solid)
		if (i == s)
			return;
	solid.push_back(s);
}

bool Entity::remSolid ( int s )
{
	unsigned int bef = solid.size();
	std::vector<int> aux;
	for (auto i: solid)
		if (i != s)
		{
			aux.push_back(i);
		}

	solid = aux;
	return bef != solid.size();
}

void Entity::addOneWayUp ( int s )
{
	upSolid.push_back(s);
}

bool Entity::remOneWayUp ( int s )
{
	unsigned int bef = upSolid.size();
	std::vector<int> aux;
	for (auto i: upSolid)
		if (i != s)
		{
			aux.push_back(i);
		}

	upSolid = aux;
	return bef != upSolid.size();
}

void Entity::addSlopeUp ( int s, Vect a, Vect b )
{
	for (auto i: upSolidSlope)
		if (i == s)
			return;
	upSolidSlope.push_back(s);
	upSolidSlopeAngles[s].push_back(a);
	upSolidSlopeAngles[s].push_back(b);
}

bool Entity::remSlopeUp ( int s )
{
	unsigned int bef = upSolidSlope.size();
	std::vector<int> aux;
	for (auto i: upSolidSlope)
		if (i != s)
			aux.push_back(i);

	upSolidSlope = aux;
	upSolidSlopeAngles.erase(s);
	return bef != upSolid.size();
}

bool Entity::isSolid ( Vect p )
{
	if (level == nullptr)
	{
		throw Exception("Entity::isSolid level é nulo");
	}

	int tile = level->get_tile(p.x, p.y);
	if (tile < 0)
		return false;
	
	return std::find(solid.begin(), solid.end(), tile) != solid.end();
}

bool Entity::isSolidOneWayUp ( Vect p )
{
	if (level == nullptr)
	{
		throw Exception("Entity::isSolid level é nulo");
	}

	int tile = level->get_tile(p.x, p.y);
	if (tile < 0)
		return false;
	
	return std::find(upSolid.begin(), upSolid.end(), tile) != upSolid.end();
}

bool Entity::isSolidSlopeUp ( Vect p, Vect * result )
{
	if (level == nullptr)
		throw Exception("Entity::isSolidSlope level é nulo");

	
	int tile = level->get_tile(p.x, p.y);
	if (tile < 0)
		return false;

	if (upSolidSlopeAngles.find(tile) != upSolidSlopeAngles.end())
	{
		int tilesize = level->get_tilesize();
		Vect c = getCollCenter();
		Vect center[2] = {Vect(c.x, c.y - collRect.h / 2), Vect(c.x, c.y + collRect.h / 2)};
		Vect slopePos(int(p.x / tilesize) * tilesize, int(p.y / tilesize) * tilesize);
		Vect aux[2] = {upSolidSlopeAngles[tile][0], upSolidSlopeAngles[tile][1]};
		if (lineIntersects(Vect::add(slopePos, aux[0]), Vect::add(slopePos, aux[1]), center[0], center[1], result))
			return true;
	}

	return false;
}

bool Entity::isGround (  )
{
	if (ground)
	{
		ground = false;
		return true;
	}

	if (level == nullptr)
		throw Exception("Entity::isGround level é nulo");

	setSides(collRect, collPoints);

	float centerX = pos.x + collRect.x + collRect.w / 2;
	for (auto p: leftSide)
	{
		p.x += centerX;
		p.y += pos.y + 1;
		if (isSolidSlopeUp(p))
			return true;
	}

	if (vel.y == 0 && upSolid.size() != 0)
	{
		Vect q;
		for (auto p: downSide)
		{
			p.x += pos.x;
			p.y += pos.y;
			q.set(p.x, p.y-1);
			p.y += 1;
			if (isSolidOneWayUp(p) && isSolidOneWayUp(q) == false)
				return true;
		}
	}

	for (auto p: downSide)
	{
		p.x += pos.x;
		p.y += pos.y;
		p.y += 1; // 1 pixel abaixo
		// verifica se é sólido
		if (isSolid(p))
			return true;
	}

	// meio da lateral de baixo do retangulo de colisão (collRect)
	int x1 = pos.x + collRect.x;
	int y1 = pos.y + collRect.y + collRect.h;
	// +1 para detectar o tile abaixo dele
	y1 = y1 + 1;

	int x2 = pos.x + collRect.x + collRect.w;
	int y2 = y1;

	if (topLadderTile >= 0)
	{
		int levelY = (y1/level->get_tilesize())*level->get_tilesize();
		//verifica se é o topo da escada na parte de baixo (downSide)
		if ((y1 - levelY) <= topTileSize && (level->get_tile(x1,y1) == topLadderTile || level->get_tile(x2,y2) == topLadderTile))
			if (vel.y == 0)
				return true;
	}

	return false;
}

void Entity::setGround ( bool g )
{	
	ground = g;
}

bool Entity::moveToPosition ( Vect pos, float maxVelNow )
{
	Vect diff(pos.x - this->pos.x, pos.y - this->pos.y);

	float seconds = FPSManager::instance()->get_delta_sec();

	diff.normalize();
	vel.x = maxVelNow * diff.x * seconds;
	vel.y = maxVelNow * diff.y * seconds;

	this->pos.x += vel.x;
	this->pos.y += vel.y;

	bool changeX = false, changeY = false;
	if (vel.x < 0)
		changeX = int(this->pos.x) < int(pos.x);
	else if (vel.x > 0)
		changeX = int(this->pos.x) > int(pos.x);
	
	if (vel.y < 0)
		changeY = int(this->pos.y) < int(pos.y);
	else if (vel.y > 0)
		changeY = int(this->pos.y) > int(pos.y);
	
	return changeX && changeY;
}

void Entity::flipAnim ( bool hor, bool ver )
{
	for (auto & it: anim)
		it.second.flip(hor, ver);
}

//move num caminho realtivo à posição do corpo
// back é pra retornar quando chegar ao fim
void Entity::setCountPath ( int count)
{
	countPath = 0;
}

bool Entity::moveInPath (Vect pos, std::vector<Vect> & path, float maxVel, bool back )
{
	if (path.size() == 0 || countPath >= int(path.size()))
		return false;

	Vect point = Vect(path[countPath].x + pos.x, path[countPath].y + pos.y);

	if (countPath > -1 && moveToPosition(point, maxVel))
	{
		if (changeBack == false)
		{
			countPath++;
			if (countPath >= int(path.size()))
			{
				if (back)
				{
					changeBack = true;
					countPath = path.size() - 1;
				}
			}
		}
		else
		{
			countPath--;
			if (countPath < 0)
			{
				countPath = 0;
				if (back)
					changeBack = false;
			}
		}

		return true;
	}

	return false;
}


bool Entity::oneWayUpCollision ()
{
	if (!level)
		throw Exception("Entity::Erro level é nulo");

	if (vel.y <= 0)
		return false;

	Vect collPos = pos;
	for (auto p: downSide)
	{
		Vect before(Vect::add(collPos, p));
		Vect after(before);

		before.y -= vel.y;
		if (isSolidOneWayUp(before) == false && isSolidOneWayUp(after))
		{
			int y = (int(after.y) / level->get_tilesize()) * level->get_tilesize();
			after.y = y - collRect.h - 1;
			after.x = collPos.x;
			setCollPos(after);
			return true;
		}
	}

	return false;
}

bool Entity::slopeUpCollision (  )
{
	if (vel.y <= 0)
		return false;
	
	Vect result;
	float centerX = pos.x + collRect.x + collRect.w / 2;
	for (auto p: leftSide)
	{
		p.x += centerX;
		p.y += pos.y;
	
		if (isSolidSlopeUp(p, &result))
		{
			//move pra fora do slope
			result.y -= collRect.h;
			result.x = pos.x + collRect.x;
			setCollPos(result);
			return true;
		}
	}

	return false;
}

bool Entity::collisionVer (  )
{
	if (level == nullptr)
		throw Exception("Entity::"+name+" collisionVer level map é nulo");
	if (vel.y == 0)
		return false;
	
	bool ret = false;
	float beforeX = getCollPos().x;
	//setSides(collRect, collPoints);

	if (vel.y < 0)
	{
		//colisão acima da cabeça e embaixo do tile
		for (auto p: upSide)
		{
			p.x += pos.x;
			p.y += pos.y;
			if (isSolid(p))
			{
				int y = ((int(p.y) / level->get_tilesize()) + 1) * level->get_tilesize();
				//tem que arrendondar o p.y para evitar bugs
				//esse 5 não era pra estar aqui!
				setCollPos(Vect(beforeX, p.y + collRect.y + (y - p.y)));
				//level->set_tile(beforeX, getCollPos().y, '.');
				ret = true;
				break;
			}
		}
	}
	else
	{
		//colisão com a parte de baixo de entity
		for (auto p: downSide)
		{
			p.x += pos.x;
			p.y += pos.y;
			if (isSolid(p))
			{
				int y = (int(p.y) / level->get_tilesize())*level->get_tilesize();
				setCollPos(Vect(beforeX, y - collRect.h - 1));
				ret = true;
				break;
			}
		}


		//tenta colidir com algum tile one way, 
		//colidir em cima do tile e embaixo de Entity
		if (oneWayUpCollision())
			ret = true;
		/*
		// colisão com a ponta da escada
		// aqui é colisão "oneway" do lado de cima
		if (!ret && topLadderTile > -1)
			for (auto p: downSide)
			{
				p = p + pos;
				p.y += vel.y;
				p.y = round(p.y);
				int x = p.x;
				int y = p.y;
				int levelY = (y / level->get_tilesize())*level->get_tilesize();
				if (y - levelY < topTileSize)
					if (level->get_tile(x,y) == topLadderTile)
					{
						p.y = p.y - (collRect.y + collRect.h) + (levelY - int(p.y));
						p.y = floor(pos.y) - 1;
						setCollPos(p);
						setSides(collRect,collPoints);
						ret = true;
						break;
					}
			}
		*/
	}

	return ret;
}

bool Entity::collisionHor (  )
{
	if (level == nullptr)
		throw Exception("Entity::"+name+" collisionHor level map é nulo");
	if (vel.x == 0)
		return false;
	bool ret = false;
	float beforeY = getCollPos().y;
	//setSides(collRect, collPoints);

	if (vel.x < 0)
	{
		for (auto p: leftSide)
		{
			p.x += pos.x;
			p.y += pos.y;
			if (isSolid(p))
			{
				int x = (int(p.x) / level->get_tilesize() + 1) * level->get_tilesize();
				p.x = x;
				setCollPos(Vect(p.x, beforeY));
				ret = true;
				break;
			}
		}
	}
	else
	{
		for (auto p: rightSide)
		{
			p.x += pos.x;
			p.y += pos.y;
			if (isSolid(p))
			{
				int x = (int(p.x) / level->get_tilesize()) * level->get_tilesize();
				p.x = int(x - collRect.w - 1);
				setCollPos(Vect(p.x,beforeY));
				ret = true;
				break;
			}
		}
	}

	return ret;
}

std::vector<Vect> Entity::getSide ( std::string side, RelativePosition relative )
{
	std::vector<Vect> ret, ref;

	setSides(collRect, collPoints);
	if (side == "left")
	{
		ref = leftSide;
	}
	else if (side == "right")
	{
		ref = rightSide;
	}
	else if (side == "down")
	{
		ref = downSide;
	}
	else if (side == "up")
	{
		ref = upSide;
	}
	else
	{
		throw Exception("Erro: lado "+side+" não encontrado");
	}

	if (relative == RELATIVE_WORLD)
	{
		for (auto it: ref)
		{
			ret.push_back(Vect::add(pos, it));
		}
	}
	else
	{
		ret = ref;
	}

	return ret;
}

void Entity::setSides ( SDL_Rect rect, int numPoints )
{
	float size, x, y;

	if (numPoints <= 2)
		throw Exception("Entity::"+name+" setSides numPoints <= 2");

	collPoints = numPoints;

	// limpa tudo
	rightSide.clear();
	leftSide.clear();
	upSide.clear();
	downSide.clear();

	size = float(rect.h) / float(numPoints - 1);
	for (int i = 0; i < numPoints; i++)
	{
		// define o lado direito
		x = rect.x + rect.w;
		y = rect.y + float(i) * size;
		rightSide.push_back(Vect(x,y));

		// define o lado esquerdo
		x = rect.x;
		//y = i * size;
		leftSide.push_back(Vect(x,y));
	}

	size = float(rect.w) / float(numPoints - 1);
	for (int i = 0; i < numPoints; i++)
	{
		// define o lado superior
		x = rect.x + float(i) * size;
		y = rect.y;
		upSide.push_back(Vect(x,y));

		// define o lado inferior
		//x = 0;
		y = rect.y + rect.h;
		downSide.push_back(Vect(x,y));
	}

}

void Entity::drawSides ( SDL_Renderer * renderer, Camera * camera )
{
	SDL_Color color = {255,0,0,255};
	SDL_Rect rect = {0,0,5,5};

	for (auto p: rightSide)
	{
		rect.x = p.x + pos.x;
		rect.y = p.y + pos.y;
		fill_rect(renderer, camera, color, rect);
	}

	for (auto p: leftSide)
	{
		rect.x = p.x + pos.x;
		rect.y = p.y + pos.y;
		fill_rect(renderer, camera, color, rect);
	}

	color.r = 255;
	color.g = 255;
	for (auto p: downSide)
	{
		rect.x = p.x + pos.x;
		rect.y = p.y + pos.y;
		fill_rect(renderer, camera, color, rect);
	}

	// centro
	rect.x = pos.x + collRect.x + collRect.w/2;
	rect.y = pos.y + collRect.y + collRect.h/2;
	fill_rect(renderer, camera, color, rect);

}

std::vector<int> Entity::getTilesHash (  )
{
	std::vector<int> ret;

	if (!level)
	{
		std::cout<<"Entity::Erro level é nulo\n";
		return ret;
	}

	int tilesize = level->get_tilesize();
	int width = level->get_width();

	
	if (leftSide.size())
	{
		int x = (leftSide[0].x + pos.x) / tilesize;
		for (int i = leftSide.size() - 1; i > -1; i--)
		{
			int y = (leftSide[i].y + pos.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}

	
	if (rightSide.size())
	{
		int x = (rightSide[0].x + pos.x) / tilesize;
		for (int i = int(rightSide.size() - 1); i > -1; i--)
		{
			int y = (rightSide[i].y + pos.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}
	
	if (downSide.size())
	{
		int y = (downSide[0].y + pos.y) / tilesize;
		for (int i = int(downSide.size() - 1); i > -1; i--)
		{
			int x = (downSide[i].y + pos.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}

	if (upSide.size())
	{
		int y = (upSide[0].y + pos.y) / tilesize;
		for (int i = int(upSide.size() - 1); i > -1; i--)
		{
			int x = (upSide[i].y + pos.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}

	return ret;
}

void Entity::applyImpulse ( Vect impulse )
{
	vel = Vect::add(vel, impulse);
}

void Entity::moveX ( float add )
{
	bool damped = false;
	//aplica aceleração
	add = double(acc.x + add) * 1.0/double(FPSManager::instance()->get_fpsdef().rate);
	vel.x += add;
	
	//se não está aplicando desaceleração e a aceleração for zero...
	if (add == 0)
	{
		//então aplique a desaceleração
		vel.x -= vel.x * damping.x;
		damped = true;
	}
	
	if (vel.x > minVel.x && vel.x > maxVel.x)
		vel.x = maxVel.x;
	else if (vel.x < -minVel.x && vel.x < -maxVel.x)
		vel.x = -maxVel.x;
	//está abaixo do intervalo
	else if (!damped && add == 0)
	{
		vel.x -= vel.x * damping.x;
	}

	pos.x += vel.x;

	//arredonda para evitar tremores no eixo X
	pos.x = roundf(pos.x);
}

void Entity::moveY ( float add )
{
	bool damped = false;
	//aplica aceleração
	add = double(acc.y + add) * 1.0/double(FPSManager::instance()->get_fpsdef().rate);
	vel.y += add;

	//se não está aplicando desaceleração e a aceleração for zero...
	if (add == 0)
	{
		//então aplique a desaceleração
		vel.y -= vel.y * damping.y;	
		damped = true;
	}
	
	if (vel.y > minVel.y && vel.y > maxVel.y)
		vel.y = maxVel.y;
	else if (vel.y < -minVel.y && vel.y < -maxVel.y)
		vel.y = -maxVel.y;
	//está abaixo do intervalo
	else if (!damped && add == 0)
	{
		vel.y -= vel.y * damping.y;
	}
	pos.y += vel.y;
	//arredonda para evitar tremores no eixo Y
	pos.y = roundf(pos.y);
}

std::string Entity::getStateString (  )
{
	return "null";
}

std::string Entity::to_json ()
{
	return "";
}

void Entity::input ( SDL_Event & event )
{

}

void Entity::draw ( SDL_Renderer * renderer, Camera * camera )
{
	if (isVisible() && currAnim)
		currAnim->draw(renderer, camera, pos.x, pos.y);
}

int Entity::update (  )
{
	return get_state();
}
