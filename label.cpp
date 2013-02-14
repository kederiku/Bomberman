#include "window.h"
#include "label.h"
#include <iostream>

Label::Label(void)
{
	this->_color.r = 150;
	this->_color.g = 150;
	this->_color.b = 150;
}

bool	Label::Init(int x, int y, const char* text)
{
	Window*		screen;
	SDL_Surface*	message;

	this->_text = text;
	screen = screen->GetInstance();
	message = TTF_RenderText_Blended(screen->Get_font(), text, this->_color);
	if (Interface::Init(x, y, message->w, message->h))
	{
		SDL_FreeSurface(message);
		return 1;
	}
	if (SDL_BlitSurface(message, 0, this->_surface, 0) == -1)
	{
		SDL_FreeSurface(message);
		return 1;
	}
	SDL_FreeSurface(message);
	return 0;
}

bool	Label::Change_color(int r, int g, int b)
{
	Window*		screen;
	SDL_Surface*	message;
	SDL_Color	color;

	color.r = r;
	color.g = g;
	color.b = b;
	screen = screen->GetInstance();
	message = TTF_RenderText_Blended(screen->Get_font(), this->_text, color);
	if (SDL_BlitSurface(message, 0, this->_surface, 0) == -1)
	{
		SDL_FreeSurface(message);
		return 1;
	}
	SDL_FreeSurface(message);
	return this->Update();
}
