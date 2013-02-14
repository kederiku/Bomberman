#ifndef _GAME_ONLINE_H_
#define _GAME_ONLINE_H_

#include "interface.h"
#include "player.h"

class	Game_online: public Interface
{
	keyboard	__key[5];
	int		__last_move;

	void		Set_key(SDL_Event* event);
	void		Send_deplacement(void);
public:
	Game_online(void);
	virtual ~Game_online(void);

	virtual bool	Read_client(void);
	bool		Get_event(SDL_Event*);
};


#endif
