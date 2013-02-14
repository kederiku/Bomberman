#ifndef _INFO_H_
#define _INFO_H_

#include "control.h"

class	Info
{
protected:
	SDL_Surface*	_sprite;
	SDL_Rect	_srcrect;
	SDL_Rect	_dstrect;
	Position	_pos;
public:
	Info(void);
	virtual ~Info(void);

	bool		Draw(SDL_Surface* screen);
	Position*	Get_pos(void);
	SDL_Rect*	Get_dest(void);
	SDL_Rect*	Get_src(void);
};


#endif
