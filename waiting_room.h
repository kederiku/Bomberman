#ifndef _WAITING_ROOM_H_
#define _WAITING_ROOM_H_

class	server;
class	Client;
class	user;
struct	Nb_player;

#include "interface.h"
#include <list>

class	Waiting_room: public Interface
{
	char	__nb_player;

	bool	Print_player(int size);
	bool	Clean_screen(void);
	bool	Add_button(char nb_player);
	int	Is_game_ready(Nb_player* data);
	bool	Refresh_screen(Nb_player* nb);
public:
	Waiting_room(void);

	virtual bool	Get_event(SDL_Event*);
	virtual bool	Read_client(void);
};

#endif
