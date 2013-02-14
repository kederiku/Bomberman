#ifndef _MAP_H_
#define _MAP_H_

#include <SDL/SDL.h>
#include "bonus.h"
#include "response.h"
#include <iostream>
#include <stdlib.h>

class	user;
class	Bomb;

struct	Case_map
{
	SDL_Rect	rect;
	bool		colision;
	bool		explose;
	bool		destructible;
	char		stat_bomb;
	Bonus*		bonus;
};

class	Map
{
	int		__size_case;
	int		__width;
	int		__height;
	Case_map**	__case;
	SDL_Surface*	__sprite_map;
	SDL_Surface*	__surface;
	Bonus		__bonus[12];
	int		__nb_bonus[12];

	bool		Create_case(int width, int height);
	void		Delete_case(void);
	void		Load_level(const char* buf);
	bool		Init_level(const char* level);
	bool		Create_bonus(void);
	unsigned	Deflagration_r(int x, int y, unsigned int size);
	unsigned	Deflagration_l(int x, int y, unsigned int size);
	unsigned	Deflagration_d(int x, int y, unsigned int size);
	unsigned	Deflagration_u(int x, int y, unsigned int size);
public:
	Map(void);
	~Map(void);

	bool		Is_wall(int x, int y);
	bool		Is_bomb(Position* pos_bomb);
	void		Change_case_map(Position* pos_bomb, unsigned int time);
	void		Init_stat_bomb(Bomb* bomb);
	void		Change_stat_bomb(SDL_Rect* _dstrect_user, Bomb* bomb, unsigned int time);
	bool		Destruct_map(void);
	bool		Init(int width, int height);
	void		Generate_bonus(Case_map* map_case);
	SDL_Surface*	Get_surface(void);
	bool		Refresh_level(void);
	int		Is_case_valide(int x, int y);
	bool		Check_destructible(int x, int y);
	bool		Check_colision(int x, int y);
	void		Delete_case(int x, int y);
	void		Check_bonus(player* user, int x, int y);
	void		Explosion(Bomb* bomb);
	unsigned	Deflagration(int add, unsigned int size);
	void		Draw_explosion(Bomb* bomb);
	bool		Get_explose(int x, int y);
	void		Change_explosion(int x, int y);
	Case_map**	Get_case(void);
	bool		Draw(Info_map map[15][13], SDL_Surface* surface);
};

#endif
