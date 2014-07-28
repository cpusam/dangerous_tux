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
		std::map <int, int> item_score; // pontuação de cada ítem de coletar, <tile, valor>
	
	public:
		CScore (  );
		virtual ~CScore (  );
		
		void set_score ( int s );
		
		void set_score_aux ( int s );
		
		int get_score (  );
		
		int get_score_aux (  );
		
		void collect_item ( int i );
};

#endif

