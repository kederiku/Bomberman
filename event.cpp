#include "event.h"
#include "timer.h"
#include <iostream>

bool	mouse_down(SDL_Event* event)
{
	return ((event->button.type == SDL_MOUSEBUTTONDOWN &&
		event->button.button == SDL_BUTTON_LEFT) ||
		event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER);
}

int	id_pollevent(SDL_Event* event)
{
	SDL_PollEvent(event);
	SDL_Delay(55);
	return 0;
}

void	event_state_mouse(int state)
{
	SDL_EventState(SDL_MOUSEMOTION, state);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, state);
	SDL_EventState(SDL_MOUSEBUTTONUP, state);
	SDL_EventState(SDL_APPMOUSEFOCUS, state);
	if (state == SDL_IGNORE)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
}

int	pollevent_without_delai(SDL_Event* event)
{
	while (SDL_PollEvent(event));
	return 0;
}
