#include "game.h"
#include "player.h"
#include "bomb.h"
#include "items.h"
#include "window.h"
#include "string.h"
#include <iostream>

Game::Game(void)
{
}

Game::~Game(void)
{
	std::vector<player*>::iterator	it;

	it = this->__player.begin();
	while (it != this->__player.end())
	{
		delete *it;
		++it;
	}
}

bool	Game::Init_player(int nb_player)
{
	player*	user;

	try
	{
		this->__player.reserve(nb_player);
	}
	catch (std::exception ex)
	{
		return 1;
	}
	user = new (std::nothrow) player();
	if (user == 0)
		return 1;
	if (user->Init("sora", 1, 1))
	{
		delete user;
		return 1;
	}
	user->Init_key(SDLK_UP, SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN, SDLK_SPACE);
	this->__player.push_back(user);
	user = new (std::nothrow) player();
	if (user == 0)
		return 1;
	if (user->Init("riku", 11, 1))
	{
		delete user;
		return 1;
	}
	user->Init_key(SDLK_w, SDLK_a, SDLK_d, SDLK_s, SDLK_LCTRL);
	this->__player.push_back(user);
	return 0;
}


bool	Game::Init(int nb_player)
{
	if (this->__map.Init(15, 13))
		return 1;
	if (this->Init_player(nb_player))
		return 1;
	if (Interface::Init(0, 0))
		return 1;
	return this->Refresh_all();
}

void	Game::Refresh_game(void)
{
	std::vector<player*>::iterator	it;
	std::list<Bomb*>::iterator	it_bomb;

	this->Color_surface(255, 255, 255);
	it = this->__player.begin();
	while (it != this->__player.end())
	{
		delete *it;
		++it;
	}
	this->__player.clear();
	it_bomb = this->__items.begin();
	while (it_bomb != this->__items.end())
	{
		delete *it_bomb;
		++it_bomb;
	}
	this->__items.clear();
	this->__map.Destruct_map();
	Init_player(2);
}

char	Game::Refresh_player(SDL_Surface* screen_game)
{
	std::vector<player*>::iterator	it_user;
	std::list<Bomb*>::iterator	it_items;

	it_user = this->__player.begin();
	while (it_user != this->__player.end())
	{
		it_items = this->__items.begin();
		while (it_items != this->__items.end())
		{
			this->__map.Change_stat_bomb((*it_user)->Get_dest(), (*it_items), (*it_items)->Get_time());
			this->__map.Change_case_map((*it_items)->Get_pos(), (*it_items)->Get_time());
			++it_items;
		}
		if ((*it_user)->Check_move(&this->__map, &this->__items) == -1)
		{
			delete *it_user;
			this->__player.erase(it_user--);
		}
		else if ((*it_user)->Draw(screen_game))
			return 1;
		++it_user;
	}
	return 0;
}

char	Game::Refresh_items(SDL_Surface* screen_game)
{
	std::list<Bomb*>::iterator	it_items;

	it_items = this->__items.begin();
	while (it_items != this->__items.end())
	{
		if ((*it_items)->Draw(&this->__map,screen_game))
			return 1;
		++it_items;
	}
	return 0;
}

char	Game::Refresh_all(void)
{
	SDL_Surface*	screen_game;
	SDL_Rect	dstrect;

	if (this->__map.Refresh_level())
		return 1;
	screen_game = this->__map.Get_surface();
	dstrect.x = (this->_surface->w - screen_game->w) / 2;
	dstrect.y = (this->_surface->h - screen_game->h) / 2;
	dstrect.w = screen_game->w;
	dstrect.h = screen_game->h;
	if (this->Refresh_items(screen_game))
		return 1;
	if (this->Refresh_player(screen_game))
		return 1;
	if (SDL_BlitSurface(screen_game, 0, this->_surface, &dstrect) == -1)
		return 1;
	if (this->__player.size() <= 1)
		return -1;
	return this->Update();
}

void		Game::Game_over(void)
{
	char	message_end[100];

	if (this->__player.size() == 0)
		id_strcpy(message_end, "draw");
	else if (this->__player.size() == 1)
	{
		id_strcpy(message_end, this->__player.front()->Get_name());
		id_strcpy(message_end + id_str_len(message_end), " win");
	}
	this->Add_text(message_end, 350, 50, 150, 150, 150);
	this->Add_text("Insert coin !", 350, 500, 150, 150, 150);
}


bool		Game::Get_event(SDL_Event* event)
{
	char				ret;
	std::vector<player*>::iterator	it_user;


	Uint8*		keystate;

	keystate = SDL_GetKeyState (NULL);
	if (keystate[SDLK_ESCAPE])
	{
		Window::GetInstance()->Change_screen(MAIN);
		this->Refresh_game();
		return 0;
	}

	this->Search_explosion();
	it_user = this->__player.begin();
	while (it_user != this->__player.end())
	{
		(*it_user)->Set_key(event);
		++it_user;
	}
	ret = this->Refresh_all();
	if (ret == 1)
		return 1;
	else if (ret == -1)
	{
		this->Game_over();
		this->Update();
		sleep(2);
		SDL_WaitEvent(event);
		Window::GetInstance()->Change_screen(GAME_OVER);
		this->Refresh_game();
	}
	return 0;
}

void	Game::Search_explosion(void)
{
	std::list<Bomb*>::iterator	it;
	unsigned int			time;

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
