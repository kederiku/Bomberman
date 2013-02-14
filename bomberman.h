#ifndef _BOMBERMAN_H_
#define _BOMBERMAN_H_

#include "player.h"
#include "map.h"
class	Bomb;
struct	Server_response;
#include <list>


class	Bomberman
{
	unsigned int		__nb_player;
	Map			__map;
	std::list<Bomb*>	__items;
	player			__player[8];
	Timer			_fps;

	void	Init_bomberman(void);
	void	Delete(void);
public:


	Bomberman(void);
	~Bomberman(void);

	bool		Init(void);
	void		Clear(void);
	void		Draw(Server_response* data, SDL_Surface* surface);
	void		Draw(Player_change* data, SDL_Surface* surface);
	bool		Init_Responseserver(Server_response* data);
	void		Move_player(int id_player, int move, bool bomb, Position* pos);
	void		Search_explosion(void);
};



#endif
