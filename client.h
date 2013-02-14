#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "buffer.h"
#include "response.h"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class	Client
{
	struct addrinfo*	__res;
	buffer			__buf_write;
	buffer			__buf_read;
	int			__sockfd;
	bool			__init;

	bool	write(void);
	bool	read(void);
public:
	Client(void);
	~Client(void);

	void	quit(void);
	void	Add_buffer(const char* str, unsigned int size);
	bool	Is_run(void);
	bool	Init(void);
	bool	Init(const char* ip, const char* port);
	bool	run(void);
	buffer*	Get_buf(void);
};

#endif
