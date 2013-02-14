#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include "event.h"
#include "interface.h"
#include "window.h"

bool	fct(SDL_Event* event, Control* cont, void*);
bool	quit(SDL_Event* event, Control*, void*);
bool	create_game(SDL_Event* event, Control*, void*);

template <id_screen id>
bool	change_screen(SDL_Event* event, Control*, void*)
{
	if (mouse_down(event))
		Window::GetInstance()->Change_screen(id);
	return 0;
}

#endif
