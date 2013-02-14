#include "bomberman.h"
#include "bomb.h"
#include "user.h"
#include "response.h"
#include <iostream>

Bomberman::Bomberman(void)
{
	this->Init_bomberman();
	this->_fps.start();
}

Bomberman::~Bomberman(void)
{
	this->Delete();
}

void	Bomberman::Init_bomberman(void)
{
}

void	Bomberman::Delete(void)
{
	std::list<Bomb*>::iterator	it;

	it = this->__items.begin();
	while (it != this->__items.end())
	{
		delete *it;
		++it;
	}
	this->__items.clear();
}

void	Bomberman::Clear(void)
{
	this->Delete();
	this->Init_bomberman();
}

bool	Bomberman::Init(void)
{
	if (this->__map.Init(15, 13))
		return 1;
	if (this->__player[0].Init("sora", 1, 1) ||
	    this->__player[1].Init("riku", 2, 1) ||
	    this->__player[2].Init("aeris", 3, 1) ||
	    this->__player[3].Init("cid", 4, 1) ||
	    this->__player[4].Init("cloud", 5, 1) ||
	    this->__player[5].Init("axel", 6 , 1) ||
	    this->__player[6].Init("xehanort", 7, 1) ||
	    this->__player[7].Init("sora", 8, 1))
		return 1;
	return 0;
}

void	Bomberman::Draw(Server_response* data, SDL_Surface* surface)
{
	char	i;

	i = 0;
	this->__map.Draw(data->map, surface);
	while (i < data->nb_player)
	{
		(this->__player + i)->Set_pos(&(data->player + i)->pos);
		(this->__player + i)->Draw(surface);
		++i;
	}
}


void	Bomberman::Draw(Player_change* data, SDL_Surface* surface)
{
	(this->__player + data->id)->Set_pos(&data->pos);
	(this->__player + data->id)->Draw(surface);
}


bool	Bomberman::Init_Responseserver(Server_response* data)
{
	Case_map**			map;
	int				x;
	int				y;
	char				i;
	std::list<Bomb*>::iterator	it;
	Position*			pos;

	if (data->game_ready == false)
		return 0;
	map = this->__map.Get_case();;
	x = 0;
	while (x < 15)
	{
		y = 0;
		while (y < 13)
		{
			data->map[x][y].src_x = map[x][y].rect.x;
			data->map[x][y].src_y = map[x][y].rect.y;
			data->map[x][y].item.time = -1;
			++y;
		}
		++x;
	}
	i = 0;
	while (i < data->nb_player)
	{
		(data->player + i)->pos.x = ((this->__player + i)->Get_dest())->x;
		(data->player + i)->pos.y = ((this->__player + i)->Get_dest())->y;
		++i;
	}
	it = this->__items.begin();
	while (it != this->__items.end())
	{
		pos = (*it)->Get_pos();
		data->map[pos->x][pos->y].item.time = 0;
		++it;
	}
	return 0;
}

void	Bomberman::Move_player(int id_player, int move, bool bomb, Position* pos)
{
	SDL_Rect*	dest;
	Uint32		time;
	int		x;
	int		y;
	Player_change	resp;

	pos->x = 0;
	pos->y = 0;
	x = 0;
	y = 0;
	time = this->_fps.get_ticks();
	dest = this->__player[id_player].Get_dest();
	if (bomb == true)
	{
		if (this->__player[id_player].Add_items(&this->__map, &this->__items))
			return;
	}
	if (time > 55)
	{
		if (move == UP)
			y -= 4;
		else if (move == LEFT)
			x -= 4;
		else if (move == RIGHT)
			x += 4;
		else if (move == DOWN)
			y += 4;
		this->_fps.start();
		if (this->__map.Is_case_valide(dest->x + x, dest->y + y) == 0)
		{
			dest->x += x;
			dest->y += y;
			this->__player[id_player].Attribute_pos(dest);
			this->__map.Check_bonus(this->__player + id_player , dest->x, dest->y);
			pos->x = dest->x;
			pos->y = dest->y;
		}
	}
}

void    Bomberman::Search_explosion(void)
{
	std::list<Bomb*>::iterator      it;
	unsigned int                    time;

	it = this->__items.begin();
	while (it != this->__items.end())
	{
		time = (*it)->Get_time();
		if (time > 3200)
		{
			this->__map.Explosion(*it);
			this->__items.erase(it--);
		}
		else if (time > 3000)
		{
			this->__map.Draw_explosion(*it);
		}
		++it;
	}
}
