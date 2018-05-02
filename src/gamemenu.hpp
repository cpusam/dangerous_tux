#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include <string>

#include "save.hpp"

#include "util.hpp"
#include "Chora.hpp"
#include "gui/label.hpp"
#include "gui/writer.hpp"

class COption: public Widget
{
	protected:
		GuiLabel * label[3]; // 
	
	public:
		COption ( std::string s, SDL_Color c[3] );
		
		~COption (  );
		
		int update (  );

		void draw ( SDL_Renderer * renderer );
};

enum EGameOptionsState
{
	ACTIVE_GAMEOPTIONS,
	INACTIVE_GAMEOPTIONS
};

class CGameOptions: public Widget
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
	
			void draw ( SDL_Renderer * renderer );
};

enum CGameMenuState
{
	INIT_GAMEMENU,
	RUNNING_GAMEMENU,
	CHOSE_OPTION,
	END_GAMEMENU
};

class CGameMenu: public StateMachine
{
	private:
		GuiLabel * header;
		COption * chose_option;
	public:
		CGameOptions options;
		
	public:
		CGameMenu ( CSaveGame *save[3] );
		
		virtual ~CGameMenu (  );
		
		void input ( SDL_Event & event );
		
		void reset (  );
		
		void draw ( SDL_Renderer * renderer );

		int update (  );
};

#endif
