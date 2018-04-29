#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "score.hpp"
#include "jetpack.hpp"
#include "gun.hpp"
#include "gameentity.hpp"

using namespace std;

struct SPlayerConfig
{
	Vect vel_max; // velocidades máximas em pé em pixels
	Vect acc; // aceleração ao se movimentar
	Vect acc_jetpack; // aceleração ao se movimentar com o jetpack
	float gravity; // aceleração da gravidade quando pulando ou caindo
	float vel_max_jump; // velocidade maxima ao começar a pular
	float vel_max_tree; // velocidade máxima na árvore
	float vel_max_jetpack; // velocidade máxima usando o jetpack
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
		int dir; // direção, 0 - esquerda e 1 - direita
		int up_key, down_key, left_key, right_key;
		int jump_key, gun_key, jetpack_key;
		int lives; // quantidade de vidas
		bool kernel; // se tem o Ã­tem "chave" da porta
		bool shot_at; // se foi baleado pelos aliens
		bool touched_alien; // se tocou um alien
		bool has_joystick; // se tem ou não joystick
		float final_pos; // posição final em X na tela de transição
		SDL_Joystick * joystick; // o joystick propriamente dito
		SDL_Rect limit; // limites de movimento do jogador
		Vect respawn; // ponto onde deve reaparecer quando morrer
		Animation * curr_anim; // animação atual
		vector <Animation> anim; // animações
		vector <int> item; // tiles que são itens de coletar
		vector <int> coll_tiles; // tiles de colisão completa
		vector <Vect> c_point; // pontos de colisão
		TileMap * map; // mapa para colisão
		
	public:
		CGun gun;
		CScore score;
		CJetpack jetpack;
		SPlayerConfig config;

	public:
		#ifndef USE_SDL2
			CPlayer (  );
		#else
			CPlayer ( SDL_Renderer * r );
		#endif
		
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
			Função que verifca e coleta itens
		*/
		bool collect_items (  );
		
		bool has_kernel (  );
		
		/*
			função que verifica e faz entrar na porta se tiver a chave
		*/
		bool enter_door (  );

		void process (  );

		void input ( SDL_Event & event );
		
		#ifndef USE_SDL2
		void draw ( Camera * cam, SDL_Surface * screen );
		#else
		void draw ( Camera * cam, SDL_Renderer * renderer );
		#endif
		
		int update (  );
};

#endif

