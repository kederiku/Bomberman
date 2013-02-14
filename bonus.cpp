#include "bonus.h"
#include "image.h"

bool	Bonus::Init(int x, int y, void (player::*fct)(void))
{
	this->__fct = fct;
	this->_srcrect.x = x;
	this->_srcrect.y = y;
	return 0;
}

void	Bonus::Give_bonus(player* user)
{
	(user->*(this->__fct))();
}
