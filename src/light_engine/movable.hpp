#ifndef MOVABLE_HPP
#define MOVABLE_HPP

class CMovable
{
	protected:
		SVect pos; // posição
		SVect vel; // velocidade
		SVect acc; // aceleração
		SDL_Rect dim; // posição e dimensão

	public:
		virtual void set_pos ( SVect p )
		{
			pos = p;
		}
		
		virtual SVect get_pos (  )
		{
			return pos;
		}
		
		virtual void set_vel ( SVect v )
		{
			vel = v;
		}
		
		virtual SVect get_vel (  )
		{
			return vel;
		}
		
		virtual void set_acc ( SVect a )
		{
			acc = a;
		}
		
		virtual SVect get_acc (  )
		{
			return acc;
		}
		
		virtual void set_dim ( SDL_Rect d )
		{
			dim = d;
		}
		
		virtual SDL_Rect get_dim (  )
		{
			return dim;
		}
};

#endif

