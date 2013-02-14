#include "button.h"
#include <iostream>

bool	Button::Get_event(SDL_Event* event)
{
	if (this->_fct(event, this, this->_data))
		return 1;
	return 0;
}
