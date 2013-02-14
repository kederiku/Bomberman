#include "text_area.h"
#include "window.h"
#include <iostream>

Text_area::Text_area(void)
{
	this->__size_text = 0;
	this->__color.r = 255;
	this->__color.g = 0;
	this->__color.b = 0;
}

Text_area::~Text_area(void)
{
	this->__text[0] = 0;
	this->__size_text = 0;
}

bool	Text_area::Init(int x, int y, int w, int h, const char* text)
{
	Window*		screen;
	SDL_Surface*	message;

	if (Interface::Init(x, y, w, h))
		return 1;
	screen = screen->GetInstance();
	message = TTF_RenderText_Blended(screen->Get_font(), text, this->__color);
	if (SDL_BlitSurface(message, 0, this->_surface, 0) == -1)
	{
		SDL_FreeSurface(message);
		return 1;
	}
	SDL_FreeSurface(message);
	this->__rect_message.x = message->w;
	this->__rect_message.y = 0;
	this->__rect_message.w = w - message->w;
	this->__rect_message.h = message->h;
	this->clear_message();
	return 0;
}

void		Text_area::clear_message(void)
{
	this->FillRect(this->__rect_message.x, this->__rect_message.y,
		       this->__rect_message.w, this->__rect_message.h, SDL_MapRGB(this->_surface->format, 0, 255, 0));
}

bool		Text_area::add_message(void)
{
	char*		text;
	SDL_Surface*	message;
	Window*		screen;

	if (this->__size_text == 0)
		return this->Update();
	text = this->__text;
	if (this->__size_text > 25)
		text += this->__size_text - 25;
	screen = screen->GetInstance();
	message = TTF_RenderText_Blended(screen->Get_font(), text, this->__color);
	if (SDL_BlitSurface(message, 0, this->_surface, &this->__rect_message) == -1)
	{
		SDL_FreeSurface(message);
		return 1;
	}
	SDL_FreeSurface(message);
	return this->Update();
}

bool		Text_area::write(char c)
{
	this->clear_message();
	this->__text[this->__size_text] = c;
	++this->__size_text;
	this->__text[this->__size_text] = 0;
	return this->add_message();
}

char		Text_area::get_key(SDL_keysym* key)
{
	if (key->sym >= SDLK_KP0 && key->sym <= SDLK_KP9)
		return key->sym - SDLK_KP0 + '0';
	return *SDL_GetKeyName(key->sym);
}

bool		Text_area::Get_event(SDL_Event* event)
{
	SDL_keysym*	keysym;

	if (event->type != SDL_KEYDOWN)
		return 0;
	SDL_EnableUNICODE(SDL_ENABLE);
	keysym = &event->key.keysym;
	if (keysym->unicode == SDLK_BACKSPACE && this->__size_text > 0)
	{
		--this->__size_text;
		this->__text[this->__size_text] = 0;
		this->clear_message();
		if (this->add_message())
			return 1;
	}
	if (keysym->unicode == SDLK_SPACE)
		this->write(' ');
	if ((keysym->unicode == SDLK_FIRST || keysym->unicode > 32) && this->__size_text < 99)
		this->write(this->get_key(keysym));
	return 0;
}

char*		Text_area::Valid_message(void)
{
	this->clear_message();
	this->__size_text = 0;
	if (this->add_message())
		return 0;
	return this->__text;
}
