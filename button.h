#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "label.h"

class	Button: public Label
{
public:
	virtual	bool	Get_event(SDL_Event*);
};

#endif
