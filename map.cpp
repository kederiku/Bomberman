#include "map.h"
#include "image.h"
#include "player.h"
#include "bomb.h"
#include "response.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

Map::Map(void)
{
	srand(time(0));
	this->__size_case = 24;
	this->__case = 0;
	this->__width = 0;
	this->__height = 0;
	this->__sprite_map = NULL;
	this->__nb_bonus[0] = 12;
	this->__nb_bonus[1] = 8;
	this->__nb_bonus[2] = 8;
	this->__nb_bonus[3] = 4;
	this->__nb_bonus[4] = 6;
	this->__nb_bonus[5] = 1;
	this->__nb_bonus[6] = 1;
	this->__nb_bonus[7] = 1;
	this->__nb_bonus[8] = 1;
	this->__nb_bonus[9] = 1;
	this->__nb_bonus[10] = 2;
	this->__nb_bonus[11] = 4;
}

Map::~Map(void)
{
	int	i;

	i = 0;
	while (i < this->__width)
	{
		delete this->__case[i];
		++i;
	}
	delete this->__case;
	SDL_FreeSurface(this->__sprite_map);
}

bool	Map::Destruct_map(void)
{
	this->__nb_bonus[0] = 12;
	this->__nb_bonus[1] = 8;
	this->__nb_bonus[2] = 8;
	this->__nb_bonus[3] = 4;
	this->__nb_bonus[4] = 6;
	this->__nb_bonus[5] = 1;
	this->__nb_bonus[6] = 1;
	this->__nb_bonus[7] = 1;
	this->__nb_bonus[8] = 1;
	this->__nb_bonus[9] = 1;
	this->__nb_bonus[10] = 2;
	this->__nb_bonus[11] = 4;
	if (this->Init_level("map.lvl"))
		return 1;
	if (this->Refresh_level())
		return 1;
	return 0;
}

bool	Map::Create_bonus(void)
{
	if (this->__bonus[0].Init(48, 48, &player::Power_up))
		return 1;
	if (this->__bonus[1].Init(0, 24, &player::Speed_up))
		return 1;
	if (this->__bonus[2].Init(48, 24, &player::Nb_bomb_up))
		return 1;
	if (this->__bonus[3].Init(120, 24, &player::Speed_down))
		return 1;
	if (this->__bonus[4].Init(120, 48, &player::Power_down))
		return 1;
	if (this->__bonus[5].Init(144, 24, &player::Power_full))
		return 1;
	if (this->__bonus[6].Init(168, 24, &player::Give_block_pass))
		return 1;
	if (this->__bonus[7].Init(168, 48, &player::Give_bomb_pass))
		return 1;
	if (this->__bonus[8].Init(24, 24, &player::Give_shit))
		return 1;
	if (this->__bonus[9].Init(72, 24, &player::Random_malus))
		return 1;
	if (this->__bonus[10].Init(144, 48, &player::Nb_bomb_down))
		return 1;
	return 0;
}

void	Map::Delete_case(void)
{
	int	i;

	i = 0;
	while (i < this->__width)
	{
		delete this->__case[i];
		++i;
	}
	this->__width = 0;
	delete this->__case;
}

bool	Map::Create_case(int width, int height)
{
	if (this->__case != 0 && this->__width == width && this->__height == height)
		return 0;
	else if (this->__case != 0)
		this->Delete_case();
	this->__case = new (std::nothrow) Case_map*[width];
	if (this->__case == 0)
		return 1;
	this->__height = height;
	while (this->__width < width)
	{
		this->__case[this->__width] = new (std::nothrow) Case_map[height];
		if (this->__case[this->__width] == 0)
			return 1;
		++this->__width;
	}
	return 0;
}

bool	Map::Init(int width, int height)
{
	if (this->Create_bonus())
		return 1;
	if (this->Create_case(width, height))
		return 1;
	this->__sprite_map = load_img("img/map.bmp");
	if (this->__sprite_map == NULL)
		return 1;
	if (this->Init_level("map.lvl"))
		return 1;
	this->__surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width * this->__size_case, height * this->__size_case, 32, 0, 0, 0, 0);
	if (this->__surface == 0)
		return 1;
	if (this->Refresh_level())
		return 1;
	return 0;
}

void	Map::Generate_bonus(Case_map* map_case)
{
	int	i;

	i = rand() % 12;
	if (i < 11 && this->__nb_bonus[i] > 0)
	{
		--this->__nb_bonus[i];
		map_case->bonus = this->__bonus + i;
	}
	else
		map_case->bonus = 0;
}

void	Map::Load_level(const char* buf)
{
	int		x;
	int		y;
	SDL_Rect	rect;

	rect.y = 0;
	rect.w = this->__size_case;
	rect.h = this->__size_case;
	y = 0;
	while (y < this->__height)
	{
		x = 0;
		while (x < this->__width)
		{
			rect.x = (*buf - '0') * this->__size_case;
			this->__case[x][y].rect = rect;
			this->__case[x][y].colision = (rect.x != 0);
			if (rect.x == this->__size_case * 2)
				this->Generate_bonus(this->__case[x] + y);
			else
				this->__case[x][y].bonus = 0;
			this->__case[x][y].destructible = (rect.x != this->__size_case);
			this->__case[x][y].explose = false;
			this->__case[x][y].stat_bomb = 0;
			++buf;
			++x;
		}
		++buf;
		++y;
	}
}

bool	Map::Init_level(const char* level)
{
	int	fd;
	int	nread;
	char	buf[256];

	fd = open(level, O_RDONLY);
	if (fd == -1)
		return 1;
	nread = read(fd, buf, 256);
	if (nread == -1 || nread != ((this->__width + 1) * this->__height))
	{
		close(fd);
		return 1;
	}
	this->Load_level(buf);
	close(fd);
	return 0;
}

bool	Map::Refresh_level(void)
{
	int		x;
	int		y;
	SDL_Rect	dstrect;

	dstrect.y = 0;
	dstrect.w = this->__size_case;
	dstrect.h = this->__size_case;
	y = 0;
	while (y < this->__height)
	{
		x = 0;
		dstrect.x = 0;
		while (x < this->__width)
		{
			if (SDL_BlitSurface(this->__sprite_map, &this->__case[x][y].rect, this->__surface, &dstrect) == -1)
				return 1;
			dstrect.x += this->__size_case;
			this->__case[x][y].explose = false;
			++x;
		}
		dstrect.y += this->__size_case;
		++y;
	}
	return 0;
}
SDL_Surface*	Map::Get_surface(void)
{
	return this->__surface;
}

int		Map::Is_case_valide(int x, int y)
{
	y += 8;
	if (!(x >= this->__size_case && x < this->__surface->w &&
	      y >= 12 && y < this->__surface->h))
		return 1;
	else if (this->__case[x / this->__size_case][y / this->__size_case].stat_bomb == 2 ||
	    this->__case[x / this->__size_case][y / this->__size_case + (y % this->__size_case != 0)].stat_bomb == 2 ||
	    this->__case[x / this->__size_case + (x % this->__size_case != 0)][y / this->__size_case].stat_bomb == 2 ||
	    this->__case[x / this->__size_case + (x % this->__size_case != 0)][y / this->__size_case + (y % this->__size_case != 0)].stat_bomb == 2)
		return 2;
	else if (this->__case[x / this->__size_case][y / this->__size_case].colision != 0 ||
	    this->__case[x / this->__size_case][y / this->__size_case + (y % this->__size_case != 0)].colision != 0 ||
	    this->__case[x / this->__size_case + (x % this->__size_case != 0)][y / this->__size_case].colision != 0 ||
	    this->__case[x / this->__size_case + (x % this->__size_case != 0)][y / this->__size_case + (y % this->__size_case != 0)].colision != 0)
	{
		if (this->__case[x / this->__size_case][y / this->__size_case].destructible &&
		    this->__case[x / this->__size_case][y / this->__size_case + (y % this->__size_case != 0)].destructible &&
		    this->__case[x / this->__size_case + (x % this->__size_case != 0)][y / this->__size_case].destructible &&
		    this->__case[x / this->__size_case + (x % this->__size_case != 0)][y / this->__size_case + (y % this->__size_case != 0)].destructible)
				return 3;
		else
			return 4;
	}
	return 0;
}

bool		Map::Check_destructible(int x, int y)
{
	if (x < 0 || y < 0 || x > this->__width || y > this->__height)
		return 0;
	return this->__case[x][y].destructible;
}

void		Map::Delete_case(int x, int y)
{
	SDL_Rect*	src;

	this->__case[x][y].colision = 0;
	this->__case[x][y].rect.x = 0;
	if (this->__case[x][y].rect.y != 0)
	{
		this->__case[x][y].rect.x = 0;
		this->__case[x][y].rect.y = 0;
		this->__case[x][y].bonus = 0;
	}
	else if (this->__case[x][y].bonus != 0)
	{
		src = this->__case[x][y].bonus->Get_src();
		this->__case[x][y].rect.x = src->x;
		this->__case[x][y].rect.y = src->y;
	}
}

void		Map::Check_bonus(player* user, int x, int y)
{
	y += 8;
	if (x % 24 != 0 || y % 24 != 0)
		return;
	x /= 24;
	y /= 24;
	if (this->__case[x][y].bonus != 0 && this->__case[x][y].rect.y != 0)
	{
		this->__case[x][y].bonus->Give_bonus(user);
		this->__case[x][y].rect.x = 0;
		this->__case[x][y].rect.y = 0;
		this->__case[x][y].bonus = 0;
	}
}

void		Map::Draw_explosion(Bomb* bomb)
{
	Position*	pos;
	player*		user;
	unsigned	size_explosion;

	if (bomb->Get_explosion() == false)
	{
		pos = bomb->Get_pos();
		user = bomb->Get_player();
		size_explosion = user->Get_size_explosion();
		this->Delete_case(pos->x, pos->y);
		bomb->Set_explosion(this->Deflagration_l(pos->x - 1, pos->y, size_explosion),
				    this->Deflagration_r(pos->x + 1, pos->y, size_explosion),
				    this->Deflagration_u(pos->x, pos->y - 1, size_explosion),
				    this->Deflagration_d(pos->x, pos->y + 1, size_explosion));
	}
}

bool		Map::Get_explose(int x, int y)
{
	return this->__case[x][y].explose;
}

void		Map::Explosion(Bomb* bomb)
{
	player*		user;

	user = bomb->Get_player();
	user->Change_nb_bomb(-1);
	delete bomb;
}

bool		Map::Is_wall(int x, int y)
{
	return this->__case[x][y].rect.x == 48 && this->__case[x][y].rect.y == 0;
}

unsigned	Map::Deflagration_r(int x, int y, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (this->Check_destructible(x + i, y) && i < size)
	{
		if (this->Is_wall(x + i, y))
		{
			this->Delete_case(x + i, y);
			return i;
		}
		this->Delete_case(x + i, y);
		++i;
	}
	return i;
}

unsigned	Map::Deflagration_l(int x, int y, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (this->Check_destructible(x - i, y) && i < size)
	{
		if (this->Is_wall(x - i, y))
		{
			this->Delete_case(x - i, y);
			return i;
		}
		this->Delete_case(x - i, y);
		++i;
	}
	return i;
}

unsigned	Map::Deflagration_d(int x, int y, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (this->Check_destructible(x, y + i) && i < size)
	{
		if (this->Is_wall(x, y + i))
		{
			this->Delete_case(x, y + i);
			return i;
		}
		this->Delete_case(x, y + i);
		++i;
	}
	return i;
}

unsigned	Map::Deflagration_u(int x, int y, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (this->Check_destructible(x, y - i) && i < size)
	{
		if (this->Is_wall(x, y - i))
		{
			this->Delete_case(x, y - i);
			return i;
		}
		this->Delete_case(x, y - i);
		++i;
	}
	return i;
}

void	Map::Change_explosion(int x ,int y)
{
	this->__case[x][y].explose = true;
}

void		Map::Init_stat_bomb(Bomb* bomb)
{
	Position*	pos_bomb;

	pos_bomb = bomb->Get_pos();
	this->__case[pos_bomb->x][pos_bomb->y].stat_bomb = 1;
}

void		Map::Change_stat_bomb(SDL_Rect* _dstrect_user, Bomb* bomb, unsigned int time)
{
	Position*			pos_bomb;
	SDL_Rect*			_dstrect_bomb;

	pos_bomb = bomb->Get_pos();
	_dstrect_bomb = bomb->Get_dest();
	if ((((_dstrect_user->x - _dstrect_bomb->x) >= 24 || (_dstrect_user->x - _dstrect_bomb->x) <= -24) ||
		((_dstrect_user->y - _dstrect_bomb->y + 12) >= 24 || (_dstrect_user->y - _dstrect_bomb->y + 8) <= -24)) && time < 3000)
		this->__case[pos_bomb->x][pos_bomb->y].stat_bomb = 2;
	else
		this->__case[pos_bomb->x][pos_bomb->y].stat_bomb = 1;
}

void	Map::Change_case_map(Position* pos_bomb, unsigned int time)
{
	if (time > 3000)
		this->__case[pos_bomb->x][pos_bomb->y].stat_bomb = 0;
}

bool	Map::Is_bomb(Position* pos_bomb)
{
	if (this->__case[pos_bomb->x][pos_bomb->y].stat_bomb == 0)
		return false;
	else
		return true;
}

Case_map**	Map::Get_case(void)
{
	return this->__case;
}


bool	Map::Draw(Info_map map[15][13], SDL_Surface* surface)
{
	int		x;
	int		y;
	SDL_Rect	dstrect;
	SDL_Rect	srcrect;

	srcrect.w = this->__size_case;
	srcrect.h = this->__size_case;

	dstrect.y = 0;
	dstrect.w = this->__size_case;
	dstrect.h = this->__size_case;
	y = 0;
	while (y < this->__height)
	{
		x = 0;
		dstrect.x = 0;
		while (x < this->__width)
		{
			srcrect.x = map[x][y].src_x;
			srcrect.y = map[x][y].src_y;
			if (SDL_BlitSurface(this->__sprite_map, &srcrect, surface, &dstrect) == -1)
				return 1;
			dstrect.x += this->__size_case;
			this->__case[x][y].explose = false;
			++x;
		}
		dstrect.y += this->__size_case;
		++y;
	}
	return 0;
}
