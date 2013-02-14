#include "player.h"
#include "image.h"
#include "map.h"
#include "bomb.h"
#include "string.h"
#include "response.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <new>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <list>

player::player(void)
{
	this->__block_pass = false;
	this->__bomb_pass = false;
	this->__boxing_glove = false;
	this->__shit = false;
	this->__directory_sprite = "img/";
	this->__name = NULL;
	this->__nb_bomb = 0;
	this->__nb_bomb_max = 1;
	this->__size_explosion = 1;
	this->__speed_run = 4;
}

player::~player(void)
{
}

bool	player::Init(const char* sprite, int x, int y)
{
	char	sprite_name[100];

	id_strcpy(sprite_name, this->__directory_sprite);
	id_strcpy(sprite_name + id_str_len(sprite_name), sprite);
	id_strcpy(sprite_name + id_str_len(sprite_name), ".bmp");
	this->__name = sprite;
	this->_sprite = load_img(sprite_name);
	if (this->_sprite == NULL)
		return 1;
	this->_srcrect.x = (this->_sprite->w / 3);
	this->_srcrect.y = (this->_sprite->h / 4) * 2;
	this->_srcrect.w = this->_sprite->w / 3;
	this->_srcrect.h = this->_sprite->h / 4;
	this->_pos.x = x;
	this->_pos.y = y;
	this->_dstrect.x = x * 24;
	this->_dstrect.y = y * 24 - 8;
	return 0;
}

void	player::Move(int x, int y)
{
	this->_dstrect.x += x;
	this->_dstrect.y += y;
	if (x < 0)
	{
		this->_srcrect.x += 24;
		this->_srcrect.y = 96;
	}
	else if (x > 0)
	{
		this->_srcrect.x += 24;
		this->_srcrect.y = 32;
	}	
	else if (y < 0)
	{
		this->_srcrect.x += 24;
		this->_srcrect.y = 0;
	}
	else if (y > 0)
	{
		this->_srcrect.x += 24;
		this->_srcrect.y = 64;
	}
	else if (x == 0 && y == 0)
		this->_srcrect.x = 24;
	if(_srcrect.x == 72)
		this->_srcrect.x = 0;
	this->_pos.x = this->_dstrect.x / 24 + ((this->_dstrect.x % 24) >= 12);
	this->_pos.y = (this->_dstrect.y + 8) / 24 + (((this->_dstrect.y + 8) % 24) >= 12);
}

unsigned int	player::Get_nb_bomb(void)
{
	return this->__nb_bomb;
}

unsigned int	player::Get_size_explosion(void)
{
	return this->__size_explosion;
}

void		player::Change_nb_bomb(int nb)
{
	this->__nb_bomb += nb;
}

void		player::Power_down(void)
{
	--this->__size_explosion;
	if (this->__size_explosion <= 0)
		this->__size_explosion = 1;
}

void		player::Power_full(void)
{
	this->__size_explosion = 15;
}

void		player::Power_up(void)
{
	if (this->__size_explosion < 10)
		++this->__size_explosion;
}

void		player::Speed_down(void)
{
	if (this->__speed_run > 2)
		this->__speed_run -= 2;
}

void		player::Speed_up(void)
{
	if (this->__speed_run < 8)
		this->__speed_run += 2;
}

void		player::Nb_bomb_up(void)
{
	if (this->__nb_bomb_max < 5)
		++this->__nb_bomb_max;
}

void		player::Nb_bomb_down(void)
{
	if (this->__nb_bomb_max > 1)
		--this->__nb_bomb_max;
}

void		player::Give_shit(void)
{
	this->__block_pass = false;
	this->__bomb_pass = false;
	this->__boxing_glove = false;
	this->__shit = true;
}

void		player::Give_boxing_glove(void)
{
	this->__block_pass = false;
	this->__bomb_pass = false;
	this->__boxing_glove = true;
	this->__shit = false;
}

void		player::Give_bomb_pass(void)
{
	this->__block_pass = false;
	this->__bomb_pass = true;
	this->__boxing_glove = false;
	this->__shit = false;
}

void		player::Give_block_pass(void)
{
	this->__block_pass = true;
	this->__bomb_pass = false;
	this->__boxing_glove = false;
	this->__shit = false;
}

void		player::Random_malus(void)
{
	int	i;
	void	(player::*fct[10])(void);

	i = rand() % 4;
	fct[0] = &player::Power_down;
	fct[1] = &player::Speed_down;
	fct[2] = &player::Give_shit;
	fct[3] = &player::Nb_bomb_down;
	(this->*fct[i])();
}



void		player::Init_key(int up, int left, int right, int down, int bomb)
{
	this->__key[UP].info = up;
	this->__key[UP].actif = false;
	this->__key[LEFT].info = left;
	this->__key[LEFT].actif = false;
	this->__key[RIGHT].info = right;
	this->__key[RIGHT].actif = false;
	this->__key[DOWN].info = down;
	this->__key[DOWN].actif = false;
	this->__key[BOMB].info = bomb;
	this->__key[BOMB].actif = false;
}

void		player::Set_key(SDL_Event* event)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (this->__key[i].info == event->key.keysym.sym)
		{
			if (event->type == SDL_KEYDOWN)
				this->__key[i].actif = true;
			else if (event->type == SDL_KEYUP)
				this->__key[i].actif = false;
		}
		++i;
	}
}

char		player::Check_move(Map* map, std::list<Bomb*>* list_bomb)
{
	int	x;
	int	y;
	int	move;

	x = 0;
	y = 0;
	if (map->Get_explose(this->_pos.x, this->_pos.y) == true)
		return -1;
	if (this->__key[LEFT].actif == true)
		x -= this->__speed_run;
	if (this->__key[RIGHT].actif == true)
		x += this->__speed_run;
	if (this->__key[UP].actif == true)
		y -= this->__speed_run;
	if (this->__key[DOWN].actif == true)
		y += this->__speed_run;
	if (this->__key[BOMB].actif == true || this->__shit == true)
	{
		if (this->Add_items(map, list_bomb))
			return 1;
	}
	move = map->Is_case_valide(this->_dstrect.x + x, this->_dstrect.y + y);
	if (move == 0)
	{
		this->Move(x, y);
		map->Check_bonus(this, this->_dstrect.x, this->_dstrect.y);
	}
	else if (this->__block_pass == true && move == 3)
		this->Move(x, y);
	else if (this->__bomb_pass == true && move == 2)
		this->Move(x, y);
	return 0;
}

Position*		player::Add_items(Map* map, std::list<Bomb*>* list_bomb)
{
	Bomb*		bomb;
	Position*	pos_bomb;

	if (this->__nb_bomb >= this->__nb_bomb_max || map->Is_wall(this->_pos.x, this->_pos.y))
		return 0;
	if (map->Is_bomb(&this->_pos) == true)
		return 0;
	bomb = new (std::nothrow) Bomb();
	if (bomb == 0 || bomb->Init(this, this->_pos.x, this->_pos.y))
	{
		delete bomb;
		return 0;
	}
	try
	{
		list_bomb->push_back(bomb);
		++this->__nb_bomb;
	}
	catch (std::exception ex)
	{
		delete bomb;
		return 0;
	}
	pos_bomb = bomb->Get_pos();
	map->Init_stat_bomb(bomb);
	return pos_bomb;
}

const char*		player::Get_name(void)
{
	return this->__name;
}

void		player::Set_pos(Position* pos)
{
	this->_dstrect.x = pos->x;
	this->_dstrect.y = pos->y;
	this->_pos.x = this->_dstrect.x / 24 + ((this->_dstrect.x % 24) >= 12);
	this->_pos.y = (this->_dstrect.y + 8) / 24 + (((this->_dstrect.y + 8) % 24) >= 12);
}

void	player::Attribute_pos(SDL_Rect* dest)
{
	this->_pos.x = dest->x / 24 + ((dest->x % 24) >= 12);
	this->_pos.y = (dest->y + 8) / 24 + (((dest->y + 8) % 24) >= 12);
}
