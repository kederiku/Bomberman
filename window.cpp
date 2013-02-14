#include "window.h"
#include "event.h"
#include "thread.h"
#include <iostream>

Window::Window(void)
{
	this->__run = true;
	this->__actif = 0;
	this->__font = 0;
	this->__server_statut = false;
	pthread_mutex_init(&this->__mutex, 0);
}

Window::~Window(void)
{
	pthread_mutex_lock(&this->__mutex);
	this->__server_statut = true;
	this->__run = false;
	pthread_mutex_unlock(&this->__mutex);
	TTF_CloseFont(this->__font);
	TTF_Quit();
	SDL_Quit();
	if (pthread_join(this->__threads[0], NULL) != 0)
		perror("join failed: ");
	if (pthread_join(this->__threads[1], NULL) != 0)
		perror("join failed: ");
}

bool	Window::Init(int w, int h)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
		return 1;
	this->_surface = SDL_SetVideoMode(w, h, 0, SDL_HWSURFACE | SDL_RESIZABLE);
	if (this->_surface == NULL)
		return 1;
	SDL_WM_SetCaption("Bomberman", NULL);
	if (TTF_Init() == -1)
		return 1;
	this->__font = TTF_OpenFont("optimus.ttf", 24);
	if (this->__font == NULL)
		return 1;
	if (this->__client.Init())
		return 1;
	if (this->__game.Init())
		return 1;
	if (this->__network.Get_game()->Init())
		return 1;
	if (pthread_create(this->__threads, 0, id_thread, this))
		return 1;
	if (pthread_create(this->__threads + 1, 0, thread_client, this))
		return 1;
	return 0;
}

TTF_Font*	Window::Get_font(void) const
{
	return this->__font;
}

SDL_Surface*	Window::Get_surface(void) const
{
	return this->_surface;
}

bool	Window::Add_screen(Interface* child, id_screen id, id_screen prev)
{
	Interface*	screen;

	screen =  new (std::nothrow) Interface();
	if (screen == 0 || screen->Init(0, 0, this->_surface->w, this->_surface->h) ||
	    screen->Add_child(child) || this->Add_child(screen))
	{
		delete child;
		delete screen;
		return 1;
	}
	screen->Set_id(id, prev);
	this->__actif = screen;
	return 0;
}

bool		Window::Change_screen(id_screen id)
{
	std::list<Control*>::iterator	it;
	Interface*			screen;

	it = this->_child.begin();
	while (it != this->_child.end())
	{
		screen = (Interface*)*it;
		if (screen->Get_id() == id && this->__actif != *it)
		{
			if (id == LOCAL_GAME)
				event_state_mouse(SDL_IGNORE);
			else
				event_state_mouse(SDL_ENABLE);
			this->__actif = screen;
			return this->__actif->Update();
		}
		++it;
	}
	return 0;
}

int		Window::Get_escape(void)
{
	Uint8*		keystate;

	keystate = SDL_GetKeyState (NULL);
	if (keystate[SDLK_ESCAPE])
	{
		if (this->__actif->Get_id() == LOCAL_GAME)
			return -1;
		if (this->__actif->Get_id() == MAIN)
			return 1;
		if (this->__network.Is_run())
		{
			pthread_mutex_lock(&this->__mutex);
			this->__server_statut = true;
			pthread_mutex_unlock(&this->__mutex);
		}
		if (this->__client.Is_run())
			this->__client.quit();
		this->Change_screen(this->__actif->Get_id_prev());
		return 0;
	}
	return -1;
}

bool		Window::Launch_callback(SDL_Event* event)
{
	Interface*	tmp;
	int		ret;

	if (this->__actif == 0)
		return 0;
	ret = this->Get_escape();
	if (ret != -1)
		return ret;
	if ((event->type == SDL_KEYDOWN || event->type == SDL_KEYUP))
		return this->__actif->Get_event(event);
	tmp = (Interface*)this->Get_control(event->button.x, event->button.y);
	if (tmp != 0)
	{
		this->__actif->Change_focus(tmp);
		return tmp->Get_event(event);
	}
	return 0;
}

void		Window::Wait_event(void)
{
	bool		quit;
	SDL_Event	event;

	quit = false;
	while (quit == false)
	{
		this->__actif->Wait_event(&event);
		this->__actif->Read_client();
		if (this->Launch_callback(&event))
			quit = true;
		else
			quit = (event.type == SDL_QUIT);
	}
}

Control*	Window::Get_control(int x, int y)
{
	return this->__actif->Get_control(x, y);
}

server*		Window::Get_server(void)
{
	return &this->__network;
}

Client*		Window::Get_client(void)
{
	return &this->__client;
}

Bomberman*	Window::Get_game(void)
{
	return &this->__game;
}

bool		Window::Read_client(void)
{
	buffer*		buf;

	if (this->__client.Is_run() == false)
		return 0;
	buf = this->__client.Get_buf();
	if (buf->size() == 0)
		return 0;
	return 0;
}

bool		Window::Is_run(void)
{
	bool	ret;

	ret = this->__run;
	return ret;
}

bool		Window::Get_server_statut(void)
{
	bool	ret;

	ret = this->__server_statut;
	return ret;
}


void		Window::Set_server_statut(bool statut)
{
	pthread_mutex_lock(&this->__mutex);
	this->__server_statut = statut;
	pthread_mutex_unlock(&this->__mutex);
}
