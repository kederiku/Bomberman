#include "waiting_room.h"
#include "user.h"
#include "window.h"
#include "callback.h"
#include "string.h"
#include "response.h"
#include "event.h"
#include "button.h"
#include <iostream>

Waiting_room::Waiting_room(void)
{
}

bool	Waiting_room::Print_player(int size)
{
	char	message[100];

	id_itoa(message, size);
	id_strcpy(message + id_str_len(message), " player online");
	if (this->Add_text(message, 300, 175, 0, 0, 0))
		return 1;
	return 0;
}

bool	Waiting_room::Clean_screen(void)
{
	std::list<Control*>::iterator	it;

	it = this->_child.begin();
	while (it != this->_child.end())
	{
		delete *it;
		++it;
	}
	this->_child.clear();
	this->Color_surface(255, 255, 255);
	return 0;
}

bool	Waiting_room::Get_event(SDL_Event*)
{
	return 0;
}

bool	Waiting_room::Add_button(char nb_player)
{
	Button*		launch_button;

	if (nb_player > 0 && Window::GetInstance()->Get_server()->Is_run())
	{
		launch_button = new (std::nothrow) Button();
		if (launch_button == 0 || launch_button->Init(300, 500, "Launch Game") || this->Add_child(launch_button))
		{
			delete launch_button;
			return 1;
		}
		launch_button->Set_callback(create_game, this);
	}
	return 0;
}

int	Waiting_room::Is_game_ready(Nb_player* data)
{
	if (data->game_ready == false)
		return -1;
	Window::GetInstance()->Change_screen(GAME_ONLINE);
	return 0;
}

bool	Waiting_room::Refresh_screen(Nb_player* nb)
{
	if (nb->nb_player == this->__nb_player)
		return 0;
	this->Color_surface(255, 255, 255);
	if (this->Add_text("WAIT PLAYER", 300, 100, 150, 150, 150))
		return 1;
	if (this->Print_player(nb->nb_player))
		return 1;
	if (this->Add_button(nb->nb_player))
		return 1;
	if (this->Update())
		return 1;
	this->__nb_player = nb->nb_player;
	return 0;
}

bool	Waiting_room::Read_client(void)
{
	int	ret;
	buffer*	buf;
	void*	data;

	ret = this->Check_connection();
	if (ret != -1)
		return ret;
	buf = Window::GetInstance()->Get_client()->Get_buf();
	data = (void*) buf->get_str();
	if (buf->size() < sizeof(Info_struct) || buf->size() < ((Info_struct*)data)->size)
		return 0;
	ret = this->Is_game_ready((Nb_player*)data);
	if (ret != -1)
		return ret;
	ret = this->Refresh_screen((Nb_player*)data);
	buf->remove(((Info_struct*)data)->size);
	return ret;
}
