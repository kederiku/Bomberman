#ifndef _BOMB_H_
#define _BOMB_H_

#include "items.h"

class	Map;
class	player;

class	Bomb: public Items
{
	player*		_player;
	char		__left;
	char		__right;
	char		__up;
	char		__down;
	bool		__explose;
public:
	bool		Init(player* user, int x, int y);
	int		Get_time(void);
	void		Explose(void);
	bool	Draw(Map* map, SDL_Surface* screen);
	player*		Get_player(void);
	void		Set_explosion(char l, char r, char u, char d);
	bool		Draw_explosion(Map* map, SDL_Surface* screen);
	bool		Deflagration_c(Map* map, SDL_Surface* screen);
	bool		Deflagration_r(Map* map, SDL_Surface* screen);
	bool		Deflagration_l(Map* map, SDL_Surface* screen);
	bool		Deflagration_d(Map* map, SDL_Surface* screen);
	bool		Deflagration_u(Map* map, SDL_Surface* screen);
	bool		Get_explosion(void);
};

#endif
