#include "score.hpp"

CScore::CScore (  )
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

CScore::~CScore (  )
{
}

void CScore::set_score ( int s )
{
	score = s;
}

void CScore::set_score_aux ( int s )
{
	score_aux = s;
}

int CScore::get_score (  )
{
	return score;
}

int CScore::get_score_aux (  )
{
	return score_aux;
}

void CScore::collect_item ( int i )
{
	score += item_score[i];
}


