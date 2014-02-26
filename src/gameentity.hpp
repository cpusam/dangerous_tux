#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

struct SGameEvent;

class CGameEntity: public CStateMachine, public CMovable
{
	protected:
		string id;
		vector <SGameEvent> events; // eventos recebidos de outras entidades
	
	public:
		CGameEntity (  )
		{
		}
		
		void set_id ( string i )
		{
			id = i;
		}
		
		string get_id (  )
		{
			return id;
		}
		
		protected:
			// método usado para processar eventos vindo de outras entidades
			virtual void process (  )
			{
			}
		
		public:
			virtual void input ( SDL_Event & event )
			{
			}
		
			virtual int update (  )
			{
				return get_state();
			}
		
			virtual void draw ( CCamera * cam, SDL_Surface * screen )
			{
			}
};

#endif

