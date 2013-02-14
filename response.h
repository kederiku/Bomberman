#ifndef	_RESPONSE_H_
#define _RESPONSE_H_

#include "position.h"

enum	response_type
{
	COMPLETE,
	MAP,
	PLAYER,
	ITEM,
	INFO_GAME,
	UNKNOW
};

struct	Info_struct
{
	response_type	type;
	unsigned int	size;
};

union	Info_item
{
	int	time;
	char	direction;
};

struct	Info_user
{
	Position	pos;
	bool		alive;
	Info_user(void): alive(false){}
};

struct	Info_map
{
	unsigned char	src_x;
	unsigned char	src_y;
	Info_item	item;
};

struct	Client_response
{
	char	move;
	bool	bomb;
	Client_response(void): move(-1), bomb(false){}
};

struct	Server_response
{
	Info_struct	info;
	char		nb_player;
	bool		game_ready;
	Info_map	map[15][13];
	Info_user	player[8];
	Server_response(void)
	{
		this->nb_player = 0;
		this->info.type = COMPLETE;
		this->info.size = sizeof(*this);
	}
};

struct	Map_change
{
	Info_struct	info;
	Map_change(void)
	{
		this->info.type = MAP;
		this->info.size = sizeof(*this);
	}
};

struct	Player_change
{
	Info_struct	info;
	Position	pos;
	unsigned char	id;
	Player_change(void)
	{
		this->info.type = PLAYER;
		this->info.size = sizeof(*this);
	}
};

struct	Item_change
{
	Info_struct	info;
	Item_change(void)
	{
		this->info.type = ITEM;
		this->info.size = sizeof(*this);
	}
};

struct	Nb_player
{
	Info_struct	info;
	char		nb_player;
	bool		game_ready;
	Nb_player(void)
	{
		this->info.type = INFO_GAME;
		this->info.size = sizeof(*this);
		this->nb_player = 0;
		this->game_ready = false;
	}
};

#endif
