#ifndef __TEXT_AREA_H_
#define __TEXT_AREA_H_

#include "button.h"
#include <SDL/SDL_ttf.h>

class	Text_area: public Interface
{
	unsigned int	__size_text;
	char		__text[100];
	SDL_Color	__color;
	SDL_Rect	__rect_message;

	bool			is_text_valid(char);
	void			clear_message(void);
	bool			write(char c);
	bool			add_message(void);
	char			get_key(SDL_keysym*);
public:
	Text_area(void);
	~Text_area(void);

	char*		Valid_message(void);
	virtual bool	Get_event(SDL_Event*);
	bool		Init(int x, int y, int w, int h, const char* txt);
};

#endif

