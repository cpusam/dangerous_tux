#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include "gameevent_manager.hpp"
#include "player.hpp"
#include "level.hpp"
#include "highscore.hpp"
#include "save.hpp"
#include "gameover.hpp"
#include "gamecredits.hpp"
#include "gameintroduction.hpp"
#include "gametitle.hpp"
#include "gametransition.hpp"
#include "gamemenu.hpp"


// enumeração das telas
enum EScreenState
{
	CHORA_SCREEN,
	CREDITS_SCREEN,
	INTRODUCTION,
	TITLE_SCREEN,
	GAMEMENU_SCREEN,
	LOAD_GAME,
	SAVE_GAME,
	LOAD_LEVELS_SCREEN,
	NEXT_LEVEL_SCREEN,
	INIT_LEVEL_SCREEN,
	PAUSE_SCREEN,
	MAIN_LOOP,
	TRANSITION,
	final_msg_SCREEN,
	GAMEOVER_SCREEN,
	HIGHSCORE_SCREEN,
	GET_SCORE_NAME,
	EXIT_SCREEN
};

class CGameScreen: public CStateMachine
{
	protected:
		int curr_level; // índice do level atual
		vector <CLevel *> levels; // os levels
		#ifndef USE_SDL2
			SDL_Surface * screen;
		#else
			SDL_Window * window;
			SDL_Renderer * renderer;
		#endif
		CCamera * cam;
		CPlayer * player;
		int any_key, enter_key, pause_key;
		int tilesize;
		
		SVect chora_pos;
		CAnimation chora;
		CSaveGame * save[3];		
		CGameOver gameover;
		CGameTitle title;
		CGameCredits credits;
		CGameIntroduction introduction;
		CGameMenu * menu;
		CGameTransition transition;
		CHighScore highscore;
		CWidget widget;
		CLabel * pause;
		CLabel * name_msg;
		CLabel * final_msg;
		CTextInput * textinput;

	public:
		#ifndef USE_SDL2
			CGameScreen ( SDL_Surface * s, CCamera * c, int ts );
		#else
			CGameScreen ( SDL_Window * w, SDL_Renderer * r, CCamera * c, int ts ): gameover(r), highscore(r), title(r), credits(r), transition(r), introduction(r)
		#endif
		
		virtual ~CGameScreen (  );
		
		void clear_levels (  );
		
		void input ( SDL_Event & event );
		
		void draw (  );
		
		int update (  );
};

#endif

