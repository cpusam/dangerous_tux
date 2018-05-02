#ifndef GAMEINTRODUCTION_HPP
#define GAMEINTRODUCTION_HPP

#include "Chora.hpp"
#include "animation.hpp"
#include "gui/label.hpp"
#include "sound/soundplayer.hpp"

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

class CGameIntroduction: public StateMachine
{
	protected:
		std::vector <Animation> anim;
		SDL_Texture * browser_bg;
		SDL_Texture * keyboard_bg;
		SDL_Texture * kernel_site;
		SDL_Texture * igloo_bg;
		GuiLabel * parallel; // frase: "In a parallel universe..."
		GuiLabel * later; // frase "Some time later..."
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
		CGameIntroduction ( SDL_Renderer * r );

		virtual ~CGameIntroduction (  );
		
		void reset (  );

		void draw ( SDL_Renderer * renderer );

		int update (  );
};

#endif
