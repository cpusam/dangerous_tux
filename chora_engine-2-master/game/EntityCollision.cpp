#include "EntityCollision.hpp"

int intersectSide ( Entity * a, Entity * b, Vect * result )
{
	if (a->get_vel().is_zero() || b->get_vel().is_zero())
		return RectSide::NONE;
	
	SDL_Rect rectA, rectB;
	rectA = a->getCollRect();
	rectB = b->getCollRect();
	if (boundingbox(rectA, rectB) == false)
		return RectSide::NONE;
	
	
	Vect a1, a2, b1, b2;

	//vetor da esquerda de A
	a1.x = rectA.x, a1.y = rectA.y;
	a2.x = rectA.x, a2.y = rectA.y + rectA.h;
	//vetor de cima de B
	b1.x = rectB.x, b1.y = rectB.y;
	b2.x = rectB.x + rectB.w, b2.y = rectB.y;
	//colisão em cima de 'b'
	if ((a->get_vel_y() > 0 || b->get_vel_y() < 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::TOP;

	//vetor da direita de A
	a1.x = rectA.x + rectA.w, a1.y = rectA.y;
	a2.x = rectA.x + rectA.w, a2.y = rectA.y + rectA.h;
	//colisão em cima de 'b'
	if ((a->get_vel_y() > 0 || b->get_vel_y() < 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::TOP;


	//vetor da direita de A
	a1.x = rectA.x + rectA.w, a1.y = rectA.y;
	a2.x = rectA.x + rectA.w, a2.y = rectA.y + rectA.h;
	//vetor de baixo
	b1.x = rectB.x, b1.y = rectB.y + rectB.h;
	b2.x = rectB.x + rectB.w, b2.y = rectB.y + rectB.h;
	//colisão em baixo de 'b'
	if ((a->get_vel_y() < 0 || b->get_vel_y() > 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::BOTTOM;
	
	//vetor da direita de A
	a1.x = rectA.x, a1.y = rectA.y;
	a2.x = rectA.x, a2.y = rectA.y + rectA.h;
	//colisão em baixo de 'b'
	if ((a->get_vel_y() < 0 || b->get_vel_y() > 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::BOTTOM;
	
	//vetor de cima de A
	a1.x = rectA.x + rectA.w, a1.y = rectA.y;
	a2.x = rectA.x + rectA.w, a2.y = rectA.y;
	//vetor de esquerda de B
	b1.x = rectB.x, b1.y = rectB.y;
	b2.x = rectB.x, b2.y = rectB.y + rectB.h;
	if ((a->get_vel_x() > 0 || b->get_vel_x() < 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::LEFT;
	
	//vetor de baixo de A
	a1.x = rectA.x + rectA.w, a1.y = rectA.y + rectA.h;
	a2.x = rectA.x + rectA.w, a2.y = rectA.y + rectA.h;
	if ((a->get_vel_x() > 0 || b->get_vel_x() < 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::LEFT;
	
	//vetor de cima de A
	a1.x = rectA.x + rectA.w, a1.y = rectA.y;
	a2.x = rectA.x + rectA.w, a2.y = rectA.y;
	//vetor de direita de B
	b1.x = rectB.x + rectB.w, b1.y = rectB.y;
	b2.x = rectB.x + rectB.w, b2.y = rectB.y + rectB.h;
	if ((a->get_vel_x() < 0 || b->get_vel_x() > 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::RIGHT;
	
	//vetor de baixo de A
	a1.x = rectA.x + rectA.w, a1.y = rectA.y + rectA.h;
	a2.x = rectA.x + rectA.w, a2.y = rectA.y + rectA.h;
	if ((a->get_vel_x() < 0 || b->get_vel_x() > 0) && lineIntersects(a1, a2, b1, b2, result))
		return RectSide::RIGHT;
	
	return RectSide::NONE;
}