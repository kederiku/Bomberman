#include "text_box.h"
#include "text_area.h"
#include "event.h"
#include "callback.h"
#include "window.h"
#include <iostream>

Text_box::Text_box(void)
{
	this->__area = NULL;
	this->__focus = NULL;
}

Text_box::~Text_box(void)
{
}

bool	Text_box::Init(int x, int y, int w, int h, text_type type)
{
	this->__type = type;
	if (Interface::Init(x, y, w, h))
		return 1;
	if (this->Add_text("enter", 100, 200, 0, 0, 250))
		return 1;
	return 0;
}

bool	Text_box::Add_text_area(int x, int y, int w, int h, const char* name)
{
	Text_area*	child;

	child = new (std::nothrow) Text_area;
	if (child == 0 || child->Init(x, y, w, h, name) || this->Add_child(child))
	{
		delete child;
		return 1;
	}
	return 0;
}

bool	Text_box::Create_game(void)
{
	server*				network;
	std::list<Control*>::iterator	it;
	char*				port;

	it = this->_child.begin();
	network = Window::GetInstance()->Get_server();
	port = ((Text_area*)*(it))->Valid_message();
	if (network->Init(port))
	{
		this->Add_text("create game fail", 0, 100, 150, 150, 150);
		return 1;
	}
	else
	{
		Window::GetInstance()->Get_client()->Init("localhost", port);
		Window::GetInstance()->Change_screen(WAIT_PLAYER);
	}
	return 0;
}

bool	Text_box::Join_game(void)
{
	Client*				client;
	std::list<Control*>::iterator	it;
	const char*			ptr[2];

	it = this->_child.begin();
	ptr[0] = ((Text_area*)*(it))->Valid_message();
	++it;
	ptr[1] = ((Text_area*)*(it))->Valid_message();
	client = Window::GetInstance()->Get_client();
	if (client->Init(ptr[0], ptr[1]))
	{
		this->Add_text("connexion  fail", 0, 100, 150, 150, 150);
		return 1;
	}
	else
		Window::GetInstance()->Change_screen(WAIT_PLAYER);
	return 0;
}

void	Text_box::Valid(void)
{
	bool		ret;

	ret = 0;
	if (this->__type == JOIN)
		ret = this->Join_game();
	else if (this->__type == CREATE)
		ret = this->Create_game();
	if (ret)
	{
		this->Update();
		this->FillRect(0, 90, this->_surface->w, 50, SDL_MapRGB(this->_surface->format, 255, 255, 255));
	}
}

bool	Text_box::Get_event(SDL_Event* event)
{
	if (event->key.keysym.sym == SDLK_RETURN && event->type == SDL_KEYDOWN)
		this->Valid();
	else if (this->__area == NULL)
		return 0;
	else if ((event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) && this->__focus != NULL)
		return this->__focus->Get_event(event);
	else if (event->button.type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
		this->__focus = this->__area;
	return 0;
}

Control*	Text_box::Get_control(int x, int y)
{
	std::list<Control*>::iterator	it;

	it = this->_child.begin();
	while (it != this->_child.end())
	{
		if ((*it)->Is_in_area(x, y))
			this->__area = *it;
		++it;
	}
	return 0;
}
