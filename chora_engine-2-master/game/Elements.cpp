#include "Elements.hpp"
#include <algorithm>

Elements * Elements::singleton = nullptr;

Elements::Elements()
{
	currCamera = nullptr;
}

Elements::~Elements()
{
	//dtor
}

Elements * Elements::instance (  )
{
	if (singleton)
		return singleton;
	
	singleton = new Elements();
	return singleton;
}

void Elements::setCurrRenderer ( SDL_Renderer * renderer )
{
	currRenderer = renderer;
}

SDL_Renderer * Elements::getCurrRenderer (  )
{
	return currRenderer;
}

Camera * Elements::getCurrCamera (  )
{
	return currCamera;
}

void Elements::setCurrCamera ( Camera * cam )
{
	if (cam)
		currCamera = cam;
}


std::vector<Entity *> Elements::getEntities (  )
{
	return entities;
}

bool Elements::hasEntity ( Entity * e )
{
	if (e)
		for (size_t i = 0; i < entities.size(); i++)
			if (e == entities[i])
				return true;
	
	return false;
}

void Elements::addEntity ( Entity * e )
{
	if (e == nullptr)
		return;
	
	if (hasEntity(e))
		return;
	
	std::cout<<"Elements::Adicionando "<<e->getId()<<" name = "<<e->getName()<<std::endl;
	entitiesID.push_back(e->getId());
	entities.push_back(e);
	layers[e->getLayer()].push_back(e);
}

Entity * Elements::getEntity ( EntityID id )
{
	for (auto * entity: entities)
		if (entity->getId() == id)
			return entity;

	return nullptr;
}

Entity * Elements::getEntityByName ( const std::string & name )
{
	for (auto * entity: entities)
		if (entity->getName() == name)
			return entity;
	
	return nullptr;
}

std::vector<Entity *> Elements::getAllEntityByGroup ( const std::string & group )
{
	std::vector<Entity*> ret;
	for (auto * entity: entities)
		if (entity->getGroup() == group)
			ret.push_back(entity);
	
	return ret;
}


Entity * Elements::remEntity ( EntityID id )
{
	std::vector<EntityID>::iterator itID = std::find(entitiesID.begin(), entitiesID.end(), id);
	if (itID == entitiesID.end())
		return nullptr;
	
	Entity * ret = getEntity(id);
	std::vector<Entity *>::iterator itEntity = std::find(entities.begin(), entities.end(), ret);
	
	std::cout<<"Elements::Removendo "<<ret->getId()<<" name = "<<ret->getName()<<std::endl;
	entities.erase(itEntity);
	entitiesID.erase(itID);
	
	itEntity = std::find(layers[ret->getLayer()].begin(), layers[ret->getLayer()].end(), ret);
	if (itEntity != layers[ret->getLayer()].end())
		layers[ret->getLayer()].erase(itEntity);

	return ret;
}

void Elements::add ( Entity * e )
{
	instance()->addEntity(e);
}

Entity * Elements::get ( EntityID id )
{
	return instance()->getEntity(id);
}

Entity * Elements::getByName ( const std::string & name )
{
	return instance()->getEntityByName(name);
}

std::vector<Entity *> Elements::getAllEntities (  )
{
	return instance()->getEntities();
}

std::vector<Entity *> Elements::getAllByGroup ( const std::string & group )
{
	return instance()->getAllEntityByGroup(group);
}

void Elements::setRenderer ( SDL_Renderer * renderer )
{
	instance()->setCurrRenderer(renderer);
}

SDL_Renderer * Elements::getRenderer (  )
{
	SDL_Renderer * renderer = instance()->getCurrRenderer();

	if (renderer == nullptr)
		printf("Elements::Erro renderer não definido\n");
	
	return renderer;
}

Camera * Elements::getCamera (  )
{
	return instance()->getCurrCamera();
}

void Elements::setCamera ( Camera * cam )
{
	instance()->setCurrCamera(cam);
}

bool Elements::has ( Entity * e )
{
	return instance()->hasEntity(e);
}

Entity * Elements::remove ( EntityID id )
{
	return instance()->remEntity(id);
}

void Elements::clear (  )
{
	instance()->clearAll();
}

void Elements::destroy (  )
{
	if (singleton)
	{
		std::vector<Entity *> entities = instance()->getAllEntities();
		for (Entity *it: entities)
			if (it)
			{
				instance()->remEntity(it->getId());
				delete it;
			}
		instance()->clearAll();

		delete singleton;
		singleton = nullptr;
	}
}

void Elements::clearAll (  )
{
	entities.clear();
	entitiesID.clear();
}

const std::map<int, std::vector<Entity *> > & Elements::getEntitiesLayers (  )
{
	return layers;
}

std::vector<EntityID> Elements::getIDs (  )
{
	return entitiesID;
}

void Elements::notifyGroup ( Entity * sender, const std::string & mesg, const std::string & group )
{
	if (group == "ALL")
	{
		std::vector<Entity*> all = instance()->getAllEntities();
		for (auto * it: all)
			it->receive(sender, mesg);
	}
	else
	{
		std::vector<Entity *> entities = instance()->getAllByGroup(group);
	
		for (auto *entity: entities)
			if (entity != sender)
				entity->receive(sender, mesg);
	}
}

////////////////////////////////////////////////////////////////////////
void Elements::input ( SDL_Event & event )
{
	instance()->inputEntities(event);
}

void Elements::draw ( SDL_Renderer * renderer, Camera * camera )
{
	instance()->drawEntities(renderer, camera);
}

void Elements::update (  )
{
	instance()->updateEntities();
}

void Elements::inputEntities ( SDL_Event & event )
{
	for (size_t i = 0, size = entities.size(); i < size; ++i)
		if (entities[i])
			entities[i]->input(event);
}

void Elements::drawEntities ( SDL_Renderer * renderer, Camera * camera )
{
	std::map<int, std::vector<Entity*> > layers = instance()->getEntitiesLayers();
	
	for (auto & it: layers)
		for (auto * entity: it.second)
			if (entity && renderer && camera && entity->isVisible())
				entity->draw(renderer, camera);
}

void Elements::updateEntities (  )
{
	if (entities.size())
		for (size_t i = 0, size = entities.size(); i < size; i++)
			if (entities[i])
			{
				#ifdef DEBUG_ELEMENTS
				printf("Atualizando %s\n", entities[i]->getName().c_str());
				#endif
				entities[i]->update();
				#ifdef DEBUG_ELEMENTS
				printf("Atualizado %s\n\n", entities[i]->getName().c_str());
				#endif
			}
}

void Elements::print (  )
{
	for (size_t i = 0; i < instance()->getIDs().size(); i++)
		printf("ID[%zd] = %ld\n", i, (instance()->getIDs()[i]));
	std::vector<Entity*> entities = instance()->getEntities();
	std::cout<<"Temos "<<entities.size()<<" ENTIDADES\n";
	for (size_t i = 0; i < instance()->getEntities().size(); i++)
		if (entities[i])
		{
			std::cout<<"id = "<<entities[i]->getId()<<"|"
			         <<entities[i]->getName()<<"|ptr="
							 <<entities[i]<<std::endl;
		}
	std::cout<< "Temos "<<entities.size()<<" entidades"<<std::endl;
}
