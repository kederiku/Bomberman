#ifndef __ID_BUF_H__
#define __ID_BUF_H__

#include <string.h>

class	buffer
{
	char*	_buf;
	size_t	_size;
	size_t	_pos;
	size_t	_capa;
	size_t	_space;

public:
	buffer(void);
	~buffer();

	size_t	size() const;
	size_t	capacity() const;
	size_t	space() const;
	bool	empty() const;
	bool	full() const;
	size_t	addstr(const char* str, size_t len);
	char*	get_str(void);
	void	remove(size_t len);
	int	find(const char* str, size_t n) const;
	int	realloc(void);
	void	clear(void);
	void	swap(buffer* tmp);
	bool	Init(int capa);
	void	print(void);
};

#endif
