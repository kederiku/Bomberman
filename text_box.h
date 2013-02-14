#ifndef __TEXT_BOX_H_
#define __TEXT_BOX_H_

class	Text_area;
#include "button.h"
#include "interface.h"
#include <vector>

enum	text_type
{
	CREATE,
	JOIN
};

class	Text_box: public Interface
{
	Control*		__area;
	Control*		__focus;
	text_type		__type;

	virtual Control*	Get_control(int x, int y);
	void			Valid(void);
	bool			Join_game(void);
	bool			Create_game(void);
public:
	Text_box(void);
	virtual ~Text_box(void);

	bool		Init(int x, int y, int w, int h, text_type type);
	bool		Add_text_area(int x, int y, int w, int h, const char* name);
	virtual bool	Get_event(SDL_Event*);
};

#endif

