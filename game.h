#ifndef _GAME_H_
#define _GAME_H_

#include "interface.h"
#include "map.h"
#include <vector>
#include "control.h"

class	Bomb;
class	player;

class	Game: public Interface
{
	std::vector<player*>	__player;
	std::list<Bomb*>	__items;
	Map			__map;

	void		Game_over(void);
	void		Refresh_game(void);
	bool		Add_items(int i);
	virtual bool	Get_event(SDL_Event*);
	void		Search_explosion(void);
	bool		Init_player(int nb_player);
	char		Refresh_player(SDL_Surface* screen_game);
	char		Refresh_items(SDL_Surface* screen_game);
	char		Refresh_all(void);
public:
	Game(void);
	~Game(void);

	bool		Init(int nb_player);
};



#endif
