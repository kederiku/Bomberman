#ifndef __BONUS_H__
#define __BONUS_H__

#include "items.h"
class	player;

class	Bonus: public Items
{
	void	(player::*__fct)(void);
public:
	bool	Init(int x, int y, void (player::*fct)(void));
	void	Give_bonus(player* user);
};

#endif
