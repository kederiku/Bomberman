#include "user.h"
#include "image.h"
#include "map.h"
#include "bomb.h"
#include "string.h"
#include "response.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <new>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <list>

user::user(void)
{
	this->__fd = -1;
	this->__id = -1;
	this->__timeout = time(NULL);
}

user::~user(void)
{
	if (this->__fd != -1)
		close(this->__fd);
}

bool	user::Init(int sckfd)
{
	socklen_t	addr_len;

	if (this->__buf_write.Init(sizeof(Server_response) * 2))
		return 1;
	if (this->__buf_read.Init(sizeof(Client_response) * 50))
		return 1;
	addr_len = sizeof(this->__sockaddr);
	this->__fd = accept(sckfd, &this->__sockaddr, &addr_len);
	if (this->__fd == -1)
		return true;
	return false;
}

int	user::Get_fd(void) const
{
	return this->__fd;
}

bool		user::write(void)
{
	int	nsend;

	nsend = send(this->__fd, this->__buf_write.get_str(), this->__buf_write.size(), MSG_NOSIGNAL);
	if (nsend >= 0)
	{
		this->__buf_write.remove(nsend);
		return 0;
	}
	return 1;
}

bool	user::read(void)
{
	char	buf[this->__buf_read.capacity()];
	int	nrecv;

	if (this->__id == -1)
		return 0;
	nrecv = recv(this->__fd, buf, this->__buf_read.capacity(), MSG_NOSIGNAL);
	if (nrecv < 1)
		return 1;
	else if (this->__timeout < 500)
		return 0;
	this->__timeout = time(NULL);
	if (this->__buf_read.size() + nrecv > this->__buf_read.capacity())
		return 1;
	this->__buf_read.addstr(buf, nrecv);
	return 0;
}

void		user::Add_buffer(const char* str, unsigned int size)
{
	this->__buf_write.addstr(str, size);
}

buffer*	user::get_buf(void)
{
	return &this->__buf_read;
}

buffer*	user::get_buf_write(void)
{
	return &this->__buf_write;
}

void	user::Set_id(int id)
{
	this->__id = id;
}

int	user::Get_id(void) const
{
	return this->__id;
}
