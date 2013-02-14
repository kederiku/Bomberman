#include "menu.h"
#include "server.h"
#include "event.h"
#include "waiting_room.h"
#include "window.h"
#include <stdio.h>
#include <iostream>

bool	fct(SDL_Event* event, Control* cont, void*)
{
	if (mouse_down(event))
		return cont->Update();
	return 0;
}


bool	quit(SDL_Event* event, Control*, void*)
{
	if (mouse_down(event))
		return 1;
	return 0;
}

bool	create_game(SDL_Event* event, Control*, void*)
{
	if (mouse_down(event))
	{
		Window::GetInstance()->Get_server()->Init_game();
		Window::GetInstance()->Change_screen(GAME_ONLINE);
	}
	return 0;
}
