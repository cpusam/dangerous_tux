#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

class CMenuItem: public CWidget
{
	protected:
		void (* callback) ( CWidget * item );

	public:
		CMenuItem ( string s, SDL_Color c[3] )
		{
			for (int i(0); i < 3; i++)
			{
				CLabel * l = new CLabel(s, c[i]);
				dim = l->get_dim();
				add_child(l);
			}
			callback = 0;
		}
		
		~CMenuItem (  )
		{
			for (int i(0); i < child.size(); i++)
				if (child[i])
					delete child[i];
		}

		void set_str ( string s )
		{
			CLabel * l = 0;
			for (int i(0); i < child.size(); i++)
				if (child[i])
				{
					l = (CLabel *)child[i];
					l->set_str(s);
					dim = l->get_dim();
				}
		}
		
		void set_callback ( void (* c) ( CWidget * item ))
		{
			callback = c;
		}
		
		void input ( SDL_Event & event )
		{
			SVect p;
			switch (event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (get_state() == 2)
							if (event.button.state == SDL_PRESSED)
								set_state(3);
					}
					break;
					
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (get_state() == 3)
						{
							if (event.button.state == SDL_RELEASED)
								set_state(4);
						}
					}
					break;
				
				case SDL_MOUSEMOTION:
					p.x = event.motion.x;
					p.y = event.motion.y;
					if (pointbox(p, dim))
					{
						if (get_state() == 1)
							set_state(2);
					}
					else
					{
						set_state(1);
					}
					break;
				default:
					break;
			}
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 1:
					return 1;
				case 2:
					return 2;
				case 3:
					return 3;
				case 4:
				{
					if (callback)
						callback(this);
					
					int x, y;
					SVect p;
					SDL_GetMouseState(&x, &y);
					p.x = x, p.y = y;
					
					if (pointbox(p, dim))
						set_state(2);
					
					return 4;
				}
				
				return get_state();
			}
		}
		
		void draw ( SDL_Surface * screen )
		{
			SDL_Rect d = dim;
			switch (get_state())
			{
				case 1:
					SDL_BlitSurface(static_cast<CLabel *>(get_child(0))->get_surface(), NULL, screen, &d);
					break;
				case 2:
					SDL_BlitSurface(static_cast<CLabel *>(get_child(1))->get_surface(), NULL, screen, &d);
					break;
				case 3:
					SDL_BlitSurface(static_cast<CLabel *>(child(2))->get_surface(), NULL, screen, &d);
					break;
				default:
					SDL_BlitSurface(static_cast<CLabel *>(child(0))->get_surface(), NULL, screen, &d);
					break;
			}
		}
};

class CGameMenu: public CWidget
{
	protected:
		CMenuItem * selected;

	public:
		CGameMenu (  )
		{
			selected = 0;
		}
		
		CGameMenu ( char **args, int count, SDL_Color c[3] )
		{
			selected = 0;
			if (count > 0)
			{
				for (int i(0); i < count; i++)
				{
					add_item(args[i], c);
				}
			}
		}
		
		void add_item ( string s, SDL_Color c[3] )
		{
			CMenuItem * item = new CMenuItem(s, c);
			add_child(item);
			
			if (child.size() >= 2)
			{
				SVect p;
				SDL_Rect d = child[child.size() - 2]->get_dim();
				p.x = 0;
				p.y = d.y + d.h;
				item->set_rel_pos(p);
			}
			
			int w = 0, j = -1;
			for (int i(0); i < child.size(); i++)
			{
				SDL_Rect d = child[i]->get_dim();
				
				if (d.w > w)
					j = i;
			}
			
			if (j > -1)
			{
				for (int i(0); i < child.size(); i++)
				{
					SVect p = child[j]->get_rel_pos();
					SDL_Rect d = child[j]->get_dim();
					
					p.y = child[i]->get_rel_pos().y;
					p.x = p.x + (d.w - child[i]->get_dim().w)/2.0f;
					child[i]->set_rel_pos(p);
				}
			}
		}
		
		CMenuItem * get_item ( int item )
		{
			return (CMenuItem *)get_child(item);
		}
};

#endif

