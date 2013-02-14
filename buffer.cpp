#include "buffer.h"
#include "string.h"
#include <iostream>
#include <string.h>

template <class T>
void	id_swap(T* x, T* y)
{
	T	tmp;

	tmp = *x;
	*x = *y;
	*y = tmp;
}

buffer::buffer(void)
{
	this->_buf = NULL;
	this->_size = 0;
	this->_pos = 0;
	this->_capa = 0;
	this->_space = 0;
}

bool	buffer::Init(int capa)
{
	delete [] this->_buf;
	this->_buf = new (std::nothrow) char [capa];
	this->_size = 0;
	this->_pos = 0;
	this->_capa = capa;
	this->_space = capa;
	return (this->_buf == 0);
}
buffer::~buffer()
{
	delete [] this->_buf;
}

size_t	buffer::size() const
{
	return this->_size;
}

size_t	buffer::capacity() const
{
	return this->_capa;
}

size_t	buffer::space() const
{
	return this->_space;
}

bool	buffer::empty() const
{
	if (this->_space == this->_capa)
		return true;
	return false;
}

bool	buffer::full() const
{
	if (this->_space == 0)
		return true;
	return false;
}

size_t	buffer::addstr(const char* str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len && this->_space > 0)
	{
		this->_buf[this->_size] = str[i];
		++i;
		++this->_size;
		--this->_space;
	}
	return i;
}

char*	buffer::get_str(void)
{
	return this->_buf;
}

void	buffer::remove(size_t len)
{
	size_t	i;

	i = 0;
	while (i + len < this->_size)
	{
		this->_buf[i] = this->_buf[i + len];
		++i;
	}
	this->_size -= len;
	this->_space += len;
}

int	buffer::find(const char* str, size_t n) const
{
	size_t	i;

	i = 0;
	while (i < this->_size && id_strncmp(this->_buf + i, str, n) != 0)
		++i;
	if (i != this->_size)
		return i;
	return -1;
}

int	buffer::realloc(void)
{
	char*	tmp;

	tmp = new (std::nothrow) char [this->_capa * 2];
	if (tmp == 0)
		return 1;
	this->_space += this->_capa;
	this->_capa = this->_capa * 2;
	id_strncpy(tmp, this->_buf, this->_size);
	delete [] this->_buf;
	this->_buf = tmp;
	return 0;
}

void	buffer::clear(void)
{
	this->_size = 0;
	this->_pos = 0;
	this->_space = this->_capa;
}

void	buffer::swap(buffer* tmp)
{
	id_swap(&this->_buf, &tmp->_buf);
	id_swap(&this->_size, &tmp->_size);
	id_swap(&this->_pos, &tmp->_pos);
	id_swap(&this->_capa, &tmp->_capa);
	id_swap(&this->_space, &tmp->_space);
}

void	buffer::print(void)
{
	size_t	i;

	i = this->_size;
	while (i < this->_size)
	{
		++i;
	}
}
