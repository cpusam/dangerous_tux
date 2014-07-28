#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include <string>

#include "save.hpp"

#include "chora_engine/util.hpp"
#include "chora_engine/chora.hpp"
#include "chora_engine/gui/label.hpp"
#include "chora_engine/gui/writer.hpp"

class COption: public CWidget
{
	protected:
		CLabel * label[3]; // 
	
	public:
		COption ( std::string s, SDL_Color c[3] );
		
		~COption (  );
		
		int update (  );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
};

enum EGameOptionsState
{
	ACTIVE_GAMEOPTIONS,
	INACTIVE_GAMEOPTIONS
};

class CGameOptions: public CWidget
{
	protected:
		std::vector <COption *> option; // opções
		COption * curr_option; // opção atual
		COption * chose_option;
		CSaveGame * save[3]; // profiles de saves atuais
		CSaveGame * curr_save; // profile atual sendo usado
		SDL_Color color[3]; // cores das opções

	public:
		CGameOptions ( CSaveGame * s[3] );
		
		~CGameOptions (  );
		
		void set_color ( SDL_Color c[3] );
		
		COption * get_chose_option (  );
		
		std::vector <COption *> get_option (  );
		
		COption * get_curr_option (  );
		
		CSaveGame * get_curr_save (  );
		
		void reset (  );
		
		private:
			void reset_options (  );
		
		public:
			void input ( SDL_Event & event );
	
			int update (  );
	
			#ifndef USE_SDL2
				void draw ( SDL_Surface * screen );
			#else
				void draw ( SDL_Renderer * renderer );
			#endif
};

enum CGameMenuState
{
	INIT_GAMEMENU,
	RUNNING_GAMEMENU,
	CHOSE_OPTION,
	END_GAMEMENU
};

class CGameMenu: public CStateMachine
{
	private:
		CLabel * header;
		COption * chose_option;
	public:
		CGameOptions options;
		
	public:
		CGameMenu ( CSaveGame *save[3] );
		
		virtual ~CGameMenu (  );
		
		void input ( SDL_Event & event );
		
		void reset (  );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
		int update (  );
};

#endif
