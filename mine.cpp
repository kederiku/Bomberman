#include "mine.h"
#include "player.h"

bool	Mine::Init(player* user, int x, int y)
{
	this->__explose = false;
	if (Items::Init(x, y, "img/mine.bmp"))
		return 1;
	this->_player = user;
	this->_srcrect.x =0;
	this->_srcrect.y = 0;
	this->_srcrect.w = 24;
	this->_srcrect.h = 24;
	this->_dstrect.x = this->_pos.x * 24;
	this->_dstrect.y = this->_pos.y * 24;
	this->_dstrect.w = this->_srcrect.w;
	this->_dstrect.h = this->_srcrect.h;
	this->__left = 0;
	this->__right = 0;
	this->__up = 0;
	this->__down = 0;
	return 0;
}

bool	Mine::Draw(Map* map, SDL_Surface* screen)
{
	int test = 1;
	if (test == 1)
	{
		this->_srcrect.x = 24;
		return Info::Draw(screen);
	}
	else
		return this->Draw_explosion(map, screen);
}

player*	Mine::Get_player(void)
{
	return this->_player;
}

void	Mine::Set_explosion(char l, char r, char u, char d)
{
	this->__explose = true;
	this->__left = l;
	this->__right = r;
	this->__up = u;
	this->__down = d;
}

bool	Mine::Draw_explosion(Map* map, SDL_Surface* screen)
{
	SDL_Rect	dstrect;
	SDL_Rect	srcrect;

	dstrect = this->_dstrect;
	srcrect = this->_srcrect;
	this->Deflagration_c(map, screen);
	this->_dstrect = dstrect;
	this->_srcrect = srcrect;
	this->Deflagration_r(map, screen);
	this->_dstrect = dstrect;
	this->_srcrect = srcrect;
	this->Deflagration_l(map, screen);
	this->_dstrect = dstrect;
	this->_srcrect = srcrect;
	this->Deflagration_u(map, screen);
	this->_dstrect = dstrect;
	this->_srcrect = srcrect;
	this->Deflagration_d(map, screen);
	this->_dstrect = dstrect;
	this->_srcrect = srcrect;
	return 0;
}

bool	Mine::Deflagration_c(Map* map, SDL_Surface* screen)
{
	this->_srcrect.y = 73;
	this->_srcrect.x = 0;
	this->Info::Draw(screen);
	map->Change_explosion(this->_pos.x, this->_pos.y);
	return 0;
}
 

bool    Mine::Deflagration_l(Map* map, SDL_Surface* screen)
{
	int	i;

	i = 0;
	this->_srcrect.y = 143;
	this->_srcrect.x = 24;
	while (i + 1 < this->__left)
	{
		this->_dstrect.x -= 24;
		this->Info::Draw(screen);
		++i;
		map->Change_explosion(this->_pos.x - i, this->_pos.y);
	}
	if (i < this->__left)
	{
		this->_dstrect.x -= 24;
		this->_srcrect.x = 0;
		this->Info::Draw(screen);
		map->Change_explosion(this->_pos.x - i - 1, this->_pos.y);
	}
	return 0;
}

bool    Mine::Deflagration_r(Map* map, SDL_Surface* screen)
{
	int	i;

	i = 0;
	this->_srcrect.y = 143;
	this->_srcrect.x = 24;
	while (i + 1 < this->__right)
	{
		this->_dstrect.x += 24;
		this->Info::Draw(screen);
		++i;
		map->Change_explosion(this->_pos.x + i, this->_pos.y);
	}
	if (i < this->__right)
	{
		this->_dstrect.x += 24;
		this->_srcrect.x = 72;
		this->Info::Draw(screen);
		map->Change_explosion(this->_pos.x + i + 1, this->_pos.y);
	}
	return 0;
}

bool	Mine::Deflagration_u(Map* map, SDL_Surface* screen)
{
	int	i;

	i = 0;
	this->_srcrect.y = 96;
	while (i + 1 < this->__up)
	{
		this->_dstrect.y -= 24;
		this->Info::Draw(screen);
		++i;
		map->Change_explosion(this->_pos.x, this->_pos.y - i);
	}
	if (i < this->__up)
	{
		this->_dstrect.y -= 24;
		this->_srcrect.y = 24;
		this->Info::Draw(screen);
		map->Change_explosion(this->_pos.x, this->_pos.y - i - 1);
	}
	return 0;
}

bool	Mine::Deflagration_d(Map* map, SDL_Surface* screen)
{
	int	i;

	i = 0;
	this->_srcrect.y = 96;
	while (i + 1 < this->__down)
	{
		this->_dstrect.y += 24;
		this->Info::Draw(screen);
		++i;
		map->Change_explosion(this->_pos.x, this->_pos.y + i);
	}
	if (i < this->__down)
	{
		this->_dstrect.y += 24;
		this->_srcrect.y = 120;
		this->Info::Draw(screen);
		map->Change_explosion(this->_pos.x, this->_pos.y + i + 1);
	}
	return 0;
}

bool	Mine::Get_explosion(void)
{
	return this->__explose;
}

