#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "score.hpp"
#include "jetpack.hpp"
#include "gun.hpp"
#include "gameentity.hpp"

using namespace std;

struct SPlayerConfig
{
	Vect vel_max; // velocidades m�ximas em p� em pixels
	Vect acc; // acelera��o ao se movimentar
	Vect acc_jetpack; // acelera��o ao se movimentar com o jetpack
	float gravity; // acelera��o da gravidade quando pulando ou caindo
	float vel_max_jump; // velocidade maxima ao come�ar a pular
	float vel_max_tree; // velocidade m�xima na �rvore
	float vel_max_jetpack; // velocidade m�xima usando o jetpack
	float shot_vel;

	SPlayerConfig (  )
	{
		vel_max.x = 7.5;
		vel_max.y = 14;
		acc.x = 1;
		acc.y = 1;
		acc_jetpack.x = 1;
		acc_jetpack.y = 1;
		gravity = 1;
		vel_max_jump = 14;
		vel_max_tree = 5;
		vel_max_jetpack = 5;
		shot_vel = 20;
	}
};

enum EPlayerState
{
	INACTIVE_PLAYER,
	WAITING,
	STANDING,
	WALKING,
	JUMPING,
	JETPACK,
	STANDING_TREE,
	WALKING_TREE,
	DYING_PLAYER,
	GAMEOVER,
	FINISHED_LEVEL,
	STANDING_FIRE,
	JUMPING_FIRE,
	WALKING_TRANSITION,
	JUMPING_TRANSITION
};

enum EPlayerDirection
{
	LEFT_PLAYER,
	RIGHT_PLAYER
};

class CPlayer: public CGameEntity
{
	protected:
		int dir; // dire��o, 0 - esquerda e 1 - direita
		int up_key, down_key, left_key, right_key;
		int jump_key, gun_key, jetpack_key;
		int lives; // quantidade de vidas
		bool kernel; // se tem o ítem "chave" da porta
		bool shot_at; // se foi baleado pelos aliens
		bool touched_alien; // se tocou um alien
		bool has_joystick; // se tem ou n�o joystick
		float final_pos; // posi��o final em X na tela de transi��o
		SDL_Joystick * joystick; // o joystick propriamente dito
		SDL_Rect limit; // limites de movimento do jogador
		Vect respawn; // ponto onde deve reaparecer quando morrer
		Animation * curr_anim; // anima��o atual
		vector <Animation> anim; // anima��es
		vector <int> item; // tiles que s�o itens de coletar
		vector <int> coll_tiles; // tiles de colis�o completa
		vector <Vect> c_point; // pontos de colis�o
		TileMap * map; // mapa para colis�o
		
	public:
		CGun gun;
		CScore score;
		CJetpack jetpack;
		SPlayerConfig config;

	public:
		CPlayer ( SDL_Renderer * r );
		
		virtual ~CPlayer (  );
		
		Vect get_center (  );
		
		void set_kernel ( bool h );
		
		TileMap * get_map (  );
		
		void set_map ( TileMap * m );
		
		void set_lives ( int l );
		
		int get_lives (  );
		
		void set_respawn ( Vect p );
		
		void kill (  );
		
		void reset_gamemenu (  );
		
		void reset (  );
		
		void set_transition ( TileMap * m, Vect p, float f_pos );
		
		bool has_coll_tile ( int tile );
		
		bool ground (  );
		
		bool tree (  );
		
		int collision_hor (  );
				
		int collision_ver (  );
		
		/*
			Verifica se tocou um tile que mata
		*/
		bool touch_bad_tile (  );
		
		/*
			Fun��o que verifca e coleta itens
		*/
		bool collect_items (  );
		
		bool has_kernel (  );
		
		/*
			fun��o que verifica e faz entrar na porta se tiver a chave
		*/
		bool enter_door (  );

		void process (  );

		void input ( SDL_Event & event );

		void draw ( Camera * cam, SDL_Renderer * renderer );
		
		int update (  );
};

#endif

