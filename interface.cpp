#include "interface.h"
#include "window.h"
#include "label.h"
#include "event.h"
#include <exception>
#include <iostream>

Interface::Interface(void)
{
	this->__focus = NULL;
	this->__id = OTHER;
	this->__wait_event = SDL_WaitEvent;
}

Interface::~Interface(void)
{
	SDL_FreeSurface(this->_surface);
}

bool	Interface::Init(int x, int y, int w, int h)
{
	this->_dstrect.x = x;
	this->_dstrect.y = y;
	this->_dstrect.w = w;
	this->_dstrect.h = h;
	this->_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
	if (this->_surface == 0)
		return 1;
	this->Color_surface(255, 255, 255);
	return 0;
}

bool	Interface::Init(int x, int y)
{
	Window*		screen;
	SDL_Surface*	surface;

	screen = screen->GetInstance();
	surface = screen->Get_surface();
	return this->Init(x, y, surface->w - x, surface->h - y);
}

id_screen	Interface::Get_id(void) const
{
	return this->__id;
}

id_screen	Interface::Get_id_prev(void) const
{
	return this->__id__prev;
}

void	Interface::Set_id(id_screen id, id_screen prev)
{
	this->__id = id;

	if (id == LOCAL_GAME)
		this->__wait_event = id_pollevent;
	else if (id == WAIT_PLAYER)
		this->__wait_event = pollevent_without_delai;
	else if (id == GAME_ONLINE)
		this->__wait_event = SDL_PollEvent;
	this->__id__prev = prev;
}

int	Interface::Wait_event(SDL_Event* event)
{
	return this->__wait_event(event);
}

bool		Interface::Get_event(SDL_Event* event)
{
	if (this->__focus != NULL)
		return this->__focus->Get_event(event);
	return 0;
}

bool	Interface::Add_child(Control* child)
{
	if (this->__focus == NULL)
		this->__focus = child;
	return Control::Add_child(child);
}

void	Interface::Change_focus(Control* child)
{
	this->__focus = child;
}

int	Interface::Check_connection(void)
{
	Client*		client;

	client = Window::GetInstance()->Get_client();
	if (client->Is_run() == false)
	{
		this->Color_surface(255, 255, 255);
		if (this->Add_text("echec de connexion au server", 300, 100, 150, 150, 150))
			return 1;
		return this->Update();
	}
	return -1;
}

bool	Interface::Read_client(void)
{
	if (this->__focus != NULL)
		return this->__focus->Read_client();
	return 0;
}
