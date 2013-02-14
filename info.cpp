#include "info.h"
#include <iostream>

Info::Info(void)
{
	this->_sprite = NULL;
	this->_pos.x = 1;
	this->_pos.y = 1;
	this->_srcrect.x = 24;
	this->_srcrect.y = 24;
	this->_srcrect.w = 24;
	this->_srcrect.h = 24;
	this->_dstrect.x = 24;
	this->_dstrect.y = 24;
	this->_dstrect.w = 24;
	this->_dstrect.h = 24;
}

Info::~Info(void)
{
	SDL_FreeSurface(this->_sprite);
}


bool	Info::Draw(SDL_Surface* screen)
{
	if (SDL_BlitSurface(this->_sprite, &this->_srcrect, screen, &this->_dstrect) == -1)
		return 1;
	return 0;
}

Position*	Info::Get_pos(void)
{
	return &this->_pos;
}

SDL_Rect*	Info::Get_dest(void)
{
	return &this->_dstrect;
}

SDL_Rect*	Info::Get_src(void)
{
	return &this->_srcrect;
}
