#ifndef GAMEINTRODUCTION_HPP
#define GAMEINTRODUCTION_HPP

#include "chora_engine/chora.hpp"
#include "chora_engine/animation.hpp"
#include "chora_engine/gui/label.hpp"
#include "chora_engine/sound/soundplayer.hpp"

enum EGameIntroductionState
{
	PARALLEL_UNIVERSE,
	LINUS_TYPING,
	ENTER_KEYDOWN,
	HACKED_KERNEL,
	LINUS_FACE,
	SOMETIME_LATER,
	IGLOO_BACKGROUND,
	LINUS_TALKING,
	TUX_TALKING,
	TUX_OUT,
	INACTIVE_INTRODUCTION
};

class CGameIntroduction: public CStateMachine
{
	protected:
		std::vector <CAnimation> anim;
		#ifndef USE_SDL2
			SDL_Surface * browser_bg;
			SDL_Surface * keyboard_bg;
			SDL_Surface * kernel_site;
			SDL_Surface * igloo_bg;
		#else
			SDL_Texture * browser_bg;
			SDL_Texture * keyboard_bg;
			SDL_Texture * kernel_site;
			SDL_Texture * igloo_bg;
		#endif
		CLabel * parallel; // frase: "In a parallel universe..."
		CLabel * later; // frase "Some time later..."
		int pos_hand; // posição da mão
		int init_hand; // posição inicial da mão
		int final_hand; // posição final da mão
		int vel_hand; // velocidade de movimento da mão
		
		int tux_eye_anim; // indice da animação atual dos olhos do tux
		int tux_mounth_anim; // índice da animação atual da boca do tux
		int init_tux_eye_anim; // indice da animação atual dos olhos do tux
		int init_tux_mounth_anim; // índice da animação atual da boca do tux
		
		int linus_eye_anim; // índice da animação atual dos olhos do linus
		int linus_mounth_anim; // índice da animação atual da boca do linus
		int init_linus_eye_anim; // índice da animação atual dos olhos do linus
		int init_linus_mounth_anim; // índice da animação atual da boca do linus
		int dialog_count; // contador de diálogos mostrados
	
	public:
		#ifndef USE_SDL2
			CGameIntroduction (  );
		#else
			CGameIntroduction ( SDL_Renderer * r );
		#endif
		
		virtual ~CGameIntroduction (  );
		
		void reset (  );
		
		#ifndef USE_SDL2
			void draw ( SDL_Surface * screen );
		#else
			void draw ( SDL_Renderer * renderer );
		#endif
		
		int update (  );
};

#endif
