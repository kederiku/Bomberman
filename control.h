#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "position.h"
#include <SDL/SDL.h>
#include <list>

class	Control
{
	void			Color_surface(Uint32 color) const;
	void			Put_pixel(int x, int y, Uint32 color) const;
protected:
	bool			_selected;
	SDL_Rect		_dstrect;
	SDL_Surface*		_surface;
	Control*		_parent;
	std::list<Control*>	_child;
	void*			_data;
	bool			(*_fct)(SDL_Event* event, Control* cont, void* data);

	void			FillRect(int x, int y, int w, int h, Uint32 color) const;
	void			Color_surface(int r, int g, int b) const;
public:
	Control(void);
	virtual ~Control(void);

	bool			Add_img(const char* img, int x, int y);
	bool			Add_text(const char* text, int x, int y, int r, int g, int b);
	bool			Is_in_area(int x, int y) const;
	void			Set_callback(bool (*fct)(SDL_Event*, Control*, void*), void* data);
	virtual Control*	Get_control(int x, int y);
	virtual bool		Add_child(Control* child);
	bool			Update(void);
	virtual bool		Get_event(SDL_Event*);
	bool			On_key_up(SDL_Event* event);
	void			On_key_down(SDL_Event* event);
	template <typename T>
	void 			Delete(std::list<T>* del)
	{
		typename std::list<T>::iterator	it;

		it = del->begin();
		while (it != del->end())
		{
			delete *it;
			++it;
		}
		del->clear();
	}
	virtual bool		Read_client(void);
};
#endif
