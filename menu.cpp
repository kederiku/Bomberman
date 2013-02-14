#include "menu.h"
#include "window.h"
#include "button.h"
#include <iostream>

Menu::Menu(void)
{
	this->__pos.x = 0;
	this->__pos.y = 0;
	this->__selected = NULL;
}


bool		Menu::Add_items(const char* name, bool (*fct)(SDL_Event*, Control*, void*), void* data)
{
	Button*		menu_item;

	menu_item = new (std::nothrow) Button();
	if (menu_item == 0 || menu_item->Init(this->__pos.x, this->__pos.y, name) ||
	    this->Add_child(menu_item))
	{
		delete menu_item;
		return 1;
	}
	this->__pos.y += 30;
	menu_item->Set_callback(fct, data);
	if (this->__selected == NULL)
		this->Change_selected(menu_item);
	return 0;
}

void	Menu::Up(void)
{
	std::list<Control*>::iterator	it;

	it = this->_child.begin();
	while (it != this->_child.end() && *it != this->__selected)
		++it;
	if (it != this->_child.begin())
	{
		--it;
		this->Change_selected((Button*)*it);
	}
}

void	Menu::Down(void)
{
	std::list<Control*>::iterator	it;

	it = this->_child.begin();
	while (it != this->_child.end() && (Button*)*it != this->__selected)
		++it;
	++it;
	if (it !=  this->_child.end())
		this->Change_selected((Button*)*it);
}

bool	Menu::Get_event(SDL_Event* event)
{
	if (event->type != SDL_KEYDOWN)
		return 0;
	if (event->key.keysym.sym == SDLK_UP)
		this->Up();
	else if (event->key.keysym.sym == SDLK_DOWN)
		this->Down();
	if (event->key.keysym.sym == SDLK_RETURN && this->__selected != NULL)
		return this->__selected->Get_event(event);
	return 0;
}

Control*	Menu::Get_control(int x, int y)
{
	std::list<Control*>::iterator	it;

	it = this->_child.begin();
	while (it != this->_child.end())
	{
		if ((*it)->Is_in_area(x, y))
		{
			this->Change_selected((Button*)*it);
			return *it;
		}
		++it;
	}
	return 0;
}

bool		Menu::Change_selected(Button* selected)
{
	if (this->__selected != selected)
	{
		if (this->__selected != NULL)
		{
			if (this->__selected->Change_color(150, 150, 150))
				return 1;
		}
		if (selected->Change_color(26, 65, 137))
			return 1;
		this->__selected = selected;
	}
	return 0;
}
