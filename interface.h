#ifndef _INTERFACE_H_
#define _INTERFACE_H_

enum	id_screen
{
	MAIN,
	NETWORK_GAME,
	LOCAL_GAME,
	GAME_OVER,
	OPTION,
	CREDIT,
	CREATE_GAME,
	WAIT_PLAYER,
	JOIN_GAME,
	GAME_ONLINE,
	OTHER
};

#include "control.h"

class	Interface: public Control
{
	Control*		__focus;
	id_screen		__id__prev;
	id_screen		__id;
	int			(*__wait_event)(SDL_Event* event);
protected:
	int			Check_connection(void);
public:
	Interface(void);
	virtual ~Interface(void);

	bool		Init(int x, int y, int w, int h);
	bool		Init(int x = 0, int y = 0);
	id_screen	Get_id(void) const;
	id_screen	Get_id_prev(void) const;
	void		Set_id(id_screen id, id_screen = MAIN);
	int		Wait_event(SDL_Event* event);
	virtual bool	Get_event(SDL_Event* event);
	virtual bool	Add_child(Control* child);
	void		Change_focus(Control* child);
	virtual bool	Read_client(void);
};

#endif
