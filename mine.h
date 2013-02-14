#ifndef _MINE_H_
#define _MINE_H_

#include "items.h"

class Map;
class player;

class	Mine: public Items
{
	player* 	_player;
	char		__left;
	char		__right;
	char		__up;
	char		__down;
	bool		__explose;

public:
	bool		Init(player* user, int x, int y);
	bool		Draw(Map* map, SDL_Surface* screen);
	void		Explose(void);
	void		Set_explosion(char l, char r, char u, char d);
	player*		Get_player(void);
	bool		Draw_explosion(Map* map, SDL_Surface* screen);
	bool		Deflagration_c(Map* map, SDL_Surface* screen);
	bool		Deflagration_r(Map* map, SDL_Surface* screen);
	bool		Deflagration_l(Map* map, SDL_Surface* screen);
	bool		Deflagration_d(Map* map, SDL_Surface* screen);
	bool		Deflagration_u(Map* map, SDL_Surface* screen);
	bool		Get_explosion(void);

};

#endif
