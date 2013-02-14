#include "game_online.h"
#include "response.h"
#include "window.h"
#include "image.h"
#include <iostream>

Game_online::Game_online(void)
{
	this->__key[UP].info = SDLK_UP;
	this->__key[UP].actif = false;
	this->__key[LEFT].info = SDLK_LEFT;
	this->__key[LEFT].actif = false;
	this->__key[RIGHT].info = SDLK_RIGHT;
	this->__key[RIGHT].actif = false;
	this->__key[DOWN].info = SDLK_DOWN;
	this->__key[DOWN].actif = false;
	this->__key[BOMB].info = SDLK_SPACE;
	this->__key[BOMB].actif = false;
	this->__last_move = -2;
}

Game_online::~Game_online(void)
{
}

bool	Game_online::Get_event(SDL_Event* event)
{
	this->Set_key(event);
	this->Send_deplacement();
	return this->Update();
}

void	Game_online::Set_key(SDL_Event* event)
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

void	Game_online::Send_deplacement(void)
{
	int		i;
	Client_response	response;

	response.move = -1;
	i = 0;
	while (i < 4)
	{
		if (response.move == -1 && this->__key[i].actif)
			response.move = i;
		else if (this->__key[i].actif)
		{
			response.move = -1;
			break;
		}
		++i;
	}
	response.bomb = this->__key[BOMB].actif;
	if (response.move != -1 || response.bomb == true)
		Window::GetInstance()->Get_client()->Add_buffer((const char*) &response, sizeof(Client_response));
}

bool		Game_online::Read_client(void)
{
	void*		data;
	buffer*		buf;
	Bomberman*	game;
	int		ret;

	ret = this->Check_connection();
	if (ret != -1)
		return ret;
	buf = Window::GetInstance()->Get_client()->Get_buf();
	data = (void*) buf->get_str();
	if (buf->size() < sizeof(Info_struct) || buf->size() < ((Info_struct*)data)->size)
		return 0;
	if (((Info_struct*)data)->type == COMPLETE)
	{
		game = Window::GetInstance()->Get_game();
		game->Draw((Server_response*)data, this->_surface);
	}
	else if (((Info_struct*)data)->type == PLAYER)
	{
		game = Window::GetInstance()->Get_game();
		game->Draw((Player_change*)data, this->_surface);
	}


	buf->remove(((Info_struct*)data)->size);
	return this->Update();
}
