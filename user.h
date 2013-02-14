#ifndef _USER_H_
#define _USER_H_

#include "info.h"
#include "map.h"
#include <sys/socket.h>
#include "position.h"
#include "buffer.h"
#include <list>


class	user
{
	int			__id;
	struct sockaddr		__sockaddr;
	buffer			__buf_write;
	buffer			__buf_read;
	int			__fd;
	time_t			__timeout;
public:
	user(void);
	~user(void);

	bool		Init(int sckfd);
	int		Get_fd(void) const;
	bool		read(void);
	bool		write(void);
	void		Add_buffer(const char* str, unsigned int size);
	buffer*		get_buf(void);
	buffer*		get_buf_write(void);
	void		Set_id(int id);
	int		Get_id(void) const;
};
#endif
