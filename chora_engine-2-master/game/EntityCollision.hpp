#include "../include/collision.hpp"
#include "Entity.hpp"

//se 'a' colidir com 'b', retorna em qual lado de 'b' o rect 'a' está tocando
extern int intersectSide ( Entity * a, Entity * b, Vect * result=nullptr );