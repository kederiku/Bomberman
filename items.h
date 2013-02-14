#ifndef _ITEM_H_
#define _ITEM_H_

#include "info.h"
#include "timer.h"

class	user;

class	Items: public Info
{
protected:
	Timer	_time;
public:
	bool	Init(int x, int y, const char* sprite);
};

#endif
