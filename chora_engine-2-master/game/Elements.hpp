/*
 Copyright (C) 2014-2018 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <vector>
#include <map>
#ifndef NO_THREAD_SAFE
	#include <atomic>
	#include <mutex>
#endif

#include "Entity.hpp"

class Entity;

class Elements
{
	public:
		virtual ~Elements();
		
		static Elements * instance (  );
		static void setRenderer ( SDL_Renderer * renderer );
		static SDL_Renderer * getRenderer (  );
		static Camera * getCamera (  );
		static void setCamera ( Camera * cam );
		
		static bool has ( Entity * e );
		static void add ( Entity * e ); // adiciona uma entidade
		static Entity * get ( EntityID id ); // pega uma entidade pelo id dela
		static Entity * getByName ( const std::string & name ); // pega objeto pelo nome
		static std::vector<Entity *> getAllEntities (  );
		//retorna todas entidades do group e sem nenhuma nulaou de id nulo
		static std::vector<Entity *> getAllByGroup ( const std::string & group ); // pega objeto pelo grupo
		static Entity * remove ( EntityID id ); //remove uma entity
		static void clear (  );
		static void destroy (  );

		//se group == "ALL" notifica todo mundo
		static void notifyGroup ( Entity *sender, const std::string & mesg, const std::string & group );
		
		static void input ( SDL_Event & event );
		static void draw ( SDL_Renderer * renderer, Camera * camera );
		static void update (  );
		
		static void print (  );


	
	private:
		Elements();
		void inputEntities ( SDL_Event & event );
		void drawEntities ( SDL_Renderer * renderer, Camera * camera );
		void updateEntities (  );

		void setCurrRenderer ( SDL_Renderer * renderer );
		void setCurrCamera ( Camera * cam );
		SDL_Renderer * getCurrRenderer (  );
		Camera * getCurrCamera (  );

		bool hasEntity ( Entity * e );
		std::vector<Entity *> getEntities (  );
		void addEntity ( Entity * e );
		Entity * getEntity ( EntityID id );
		Entity * getEntityByName ( const std::string & name );
		std::vector<Entity *> getAllEntityByGroup ( const std::string & group );
		Entity * remEntity ( EntityID id );
		void clearAll (  );
		const std::map<int, std::vector<Entity *> > & getEntitiesLayers (  );

		std::map<int, std::vector<Entity *> > layers;
		std::vector<EntityID> getIDs (  );
		SDL_Renderer * currRenderer;
		Camera * currCamera;
		std::vector<EntityID> entitiesID;
		std::vector<Entity *> entities;
		static Elements * singleton;
};

#endif // ELEMENTS_HPP
