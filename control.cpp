#include "control.h"
#include "label.h"
#include "image.h"
#include <iostream>

Control::Control(void)
{
	this->_selected = false;
	this->_dstrect.x = 0;
	this->_dstrect.y = 0;
	this->_dstrect.w = 0;
	this->_dstrect.h = 0;
	this->_surface = 0;
	this->_parent = 0;
	this->_data = 0;
	this->_fct = 0;
}

Control::~Control(void)
{
	std::list<Control*>::iterator	it;

	it = this->_child.begin();
	while (it != this->_child.end())
	{
		delete *it;
		++it;
	}
}

bool		Control::Get_event(SDL_Event*)
{
	return 0;
}

bool		Control::Is_in_area(int x, int y) const
{
	return (x > this->_dstrect.x && x < this->_dstrect.x + this->_dstrect.w &&
		y > this->_dstrect.y && y < this->_dstrect.y + this->_dstrect.h);
}

Control*	Control::Get_control(int x, int y)
{
	std::list<Control*>::iterator	it;
	Control*			ret;

	it = this->_child.begin();
	while (it != this->_child.end())
	{
		ret = (*it)->Get_control(x - (*it)->_dstrect.x, y - (*it)->_dstrect.y);
		if (ret != 0)
			return ret;
		if ((*it)->Is_in_area(x, y))
			return *it;
		++it;
	}
	return 0;
}

void		Control::Set_callback(bool (*fct)(SDL_Event*, Control*, void*), void* data)
{
	this->_fct = fct;
	this->_data = data;
}

void		Control::Color_surface(int r, int g, int b) const
{
	this->Color_surface(SDL_MapRGB(this->_surface->format, r, g, b));
}

void		Control::Color_surface(Uint32 color) const
{
	this->FillRect(0, 0, this->_surface->w, this->_surface->h, color);
}

void		Control::FillRect(int x, int y, int w, int h, Uint32 color) const
{
	int	i;
	int	j;

	j = 0;
	while (j < h)
	{
		i = 0;
		while (i < w)
		{
			this->Put_pixel(x + i, y + j, color);
			++i;
		}
		++j;
	}
}

void		Control::Put_pixel(int x, int y, Uint32 color) const
{
	int		bpp;
	unsigned char*	p;

	bpp = this->_surface->format->BytesPerPixel;
	p = (unsigned char*)this->_surface->pixels + y * this->_surface->pitch + x * bpp;
	*(unsigned int*)p = color;
}

bool		Control::Add_child(Control* child)
{
	child->_parent = this;
	if (SDL_BlitSurface(child->_surface, 0, this->_surface, &child->_dstrect) == -1)
		return 1;
	try
	{
		this->_child.push_back(child);
	}
	catch (std::exception ex)
	{
		return 1;
	}
	return 0;
}

bool		Control::Update(void)
{
	Control*	parent;
	Control*	child;

	child = this;
	parent = child->_parent;
	while (parent != 0)
	{
		if (SDL_BlitSurface(child->_surface, 0, parent->_surface, &child->_dstrect) == -1)
			return 1;
		SDL_UpdateRect(parent->_surface, 0, 0, parent->_dstrect.w, parent->_dstrect.h);
		child = parent;
		parent = child->_parent;
	}
	return 0;
}

void		Control::On_key_down(SDL_Event* event)
{
	if (event->type == SDL_MOUSEBUTTONDOWN)
		this->_selected = true;
}

bool		Control::On_key_up(SDL_Event* event)
{
	if (event->type == SDL_MOUSEBUTTONUP && this->_selected)
	{
		this->_selected = false;
		return 1;
	}
	return 0;
}

bool	Control::Add_text(const char* text, int x, int y, int r, int g, int b)
{
	Label*		message;

	message = new (std::nothrow) Label();
	if (message == 0 || message->Init(x, y, text) ||
	    SDL_BlitSurface(message->_surface, 0, this->_surface, &message->_dstrect) == -1)
	{
		delete message;
		return 1;
	}
	if (message->Change_color(r, g, b))
		return 1;
	return 0;
}

bool	Control::Add_img(const char* img, int x, int y)
{
	SDL_Surface*	image;
	SDL_Rect	dstrect;

	image = load_img(img);
	if (image == 0)
		return 1;
	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = image->w;
	dstrect.h = image->h;
	if (SDL_BlitSurface(image, 0, this->_surface, &dstrect) == -1)
	{
		SDL_FreeSurface(image);
		delete image;
		return 1;
	}
	SDL_FreeSurface(image);
	return 0;
}

bool		Control::Read_client(void)
{
	return 0;
}
