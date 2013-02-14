#ifndef _EVENT_H_
#define _EVENT_H_

#include <SDL/SDL.h>

int	id_pollevent(SDL_Event* event);
void	event_state_mouse(int state);
bool	mouse_down(SDL_Event* event);
int	pollevent_without_delai(SDL_Event* event);
#endif
