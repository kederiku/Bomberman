#include "items.h"
#include "image.h"

bool	Items::Init(int x, int y, const char* sprite)
{
	this->_sprite = load_img(sprite);
	if (this->_sprite == NULL)
		return 1;
	this->_pos.x = x;
	this->_pos.y = y;
	this->_time.start();
	return 0;
}
