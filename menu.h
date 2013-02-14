#ifndef __MENU_H__
#define __MENU_H__

#include "interface.h"
struct	Position;
class	Button;

class Menu: public Interface
{
	Position		__pos;
	Button*			__selected;

	virtual	bool		Get_event(SDL_Event*);
	bool			Change_selected(Button* selected);
	virtual Control*	Get_control(int x, int y);
	void			Up(void);
	void			Down(void);
public:
	Menu(void);
	bool		Add_items(const char* name, bool (*fct)(SDL_Event*, Control*, void*), void* data);
};

#endif
