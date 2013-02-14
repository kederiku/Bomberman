#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "info.h"
#include "map.h"
#include <sys/socket.h>
#include "position.h"
#include "buffer.h"
#include <list>

class	Bomb;

enum	key_info
{
	UP,
	LEFT,
	RIGHT,
	DOWN,
	BOMB,
	SPECIAL,
	NONE
};

struct	keyboard
{
	int		info;
	bool		actif;
};

class	player: public Info
{
	bool			__block_pass;
	bool			__bomb_pass;
	bool			__boxing_glove;
	bool			__shit;
	int			__id;
	const char*		__directory_sprite;
	const char*		__name;
	unsigned int		__nb_bomb;
	unsigned int		__nb_bomb_max;
	unsigned int		__size_explosion;
	unsigned int		__speed_run;
	keyboard		__key[5];

public:
	player(void);
	~player(void);

	Position*	Add_items(Map* map, std::list<Bomb*>* list_bomb);
	bool		Init(int sckfd);
	bool		Init(const char* sprite, int x = 1, int y = 1);
	void		Move(int x, int y);
	unsigned int	Get_nb_bomb(void);
	unsigned int	Get_size_explosion(void);
	void		Change_nb_bomb(int nb);
	void		Power_up(void);
	void		Power_down(void);
	void		Power_full(void);
	void		Speed_up(void);
	void		Speed_down(void);
	void		Nb_bomb_up(void);
	void		Nb_bomb_down(void);
	void		Init_key(int up, int left, int right, int down, int bomb);
	void		Set_key(SDL_Event* event);
	char		Check_move(Map* map, std::list<Bomb*>* list_bomb);
	const char*	Get_name(void);
	void		Set_pos(Position* pos);
	void		Attribute_pos(SDL_Rect* dest);
	void		Give_boxing_glove(void);
	void		Give_bomb_pass(void);
	void		Give_block_pass(void);
	void		Give_shit(void);
	void		Random_malus(void);
};

#endif
