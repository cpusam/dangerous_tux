/*
	-falta definir quanto vale cada item coletado
*/

#ifndef SCORE_HPP
#define SCORE_HPP

#include <map>

class CScore
{
	protected:
		int score; // pontuação
		int score_aux; // pontuação antiga ao ganhar uma vida
		map <int, int> item_score; // pontuação de cada ítem de coletar, <tile, valor>
	
	public:
		CScore (  )
		{
			score = score_aux = 0;
			// define o valor de cada tile
			item_score['d'] =  50;
			item_score['e'] = 100;
			item_score['f'] = 100;
			item_score['t'] = 250;
			item_score['g'] = 500;
			item_score['K'] = 750;
		}
		
		void set_score ( int s )
		{
			score = s;
		}
		
		void set_score_aux ( int s )
		{
			score_aux = s;
		}
		
		int get_score (  )
		{
			return score;
		}
		
		int get_score_aux (  )
		{
			return score_aux;
		}
		
		void collect_item ( int i )
		{
			score += item_score[i];
		}
};

#endif

