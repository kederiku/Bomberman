#ifndef _LABEL_H_
#define _LABEL_H_

#include "interface.h"
#include <SDL/SDL_ttf.h>

class	Label: public Interface
{
	const char*	_text;
	SDL_Color	_color;

public:
	Label(void);

	bool		Init(int x, int y, const char* text);
	bool		Change_color(int r, int g, int b);
};

#endif
