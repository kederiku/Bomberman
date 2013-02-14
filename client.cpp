#include "client.h"
#include "string.h"
#include "buffer.h"
#include "response.h"
#include <netdb.h>
#include <iostream>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <dlfcn.h>

Client::Client(void)
{
	this->__sockfd = -1;
	this->__init = false;
	this->__res = 0;
}

Client::~Client(void)
{
	if (this->__sockfd != -1)
		close(this->__sockfd);
	freeaddrinfo(this->__res);
}


bool	Client::Init(void)
{
	if (this->__buf_write.Init(sizeof(Client_response)))
		return 1;
	if (this->__buf_read.Init(sizeof(Server_response) * 5))
		return 1;
	return 0;
}

bool	Client::Init(const char* ip, const char* port)
{
	addrinfo	hints;

	if (this->__init == true)
		return 0;
	id_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(ip, port, &hints, &this->__res) != 0)
		return 1;
	this->__sockfd = socket(this->__res->ai_family, this->__res->ai_socktype,
			        this->__res->ai_protocol);
	if (this->__sockfd == -1)
		return 1;
	if (connect(this->__sockfd, this->__res->ai_addr,
		    this->__res->ai_addrlen) == -1)
		return 1;
	this->__init = true;
	return 0;
}

bool	Client::write(void)
{
	int	nsend;

	if (this->__buf_write.size() == 0)
		return 0;
	nsend = send(this->__sockfd, this->__buf_write.get_str(), this->__buf_write.size(), MSG_NOSIGNAL);
	if (nsend > 0)
	{
		this->__buf_write.remove(nsend);
		return 0;
	}
	return 1;
}

bool	Client::read(void)
{
	char	buf[sizeof(Server_response) * 2];
	int	nrecv;

	nrecv = recv(this->__sockfd, buf, sizeof(Server_response) * 2, MSG_NOSIGNAL);
	if (nrecv < 1)
		return 1;
	if (this->__buf_read.size() + nrecv > this->__buf_read.capacity())
		return 1;
	this->__buf_read.addstr(buf, nrecv);
	return 0;
}

bool	Client::run(void)
{
	bool		ret;
	fd_set		fdsr;
	fd_set		fdsw;
	struct timeval	timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 500;
	ret = false;
	if (this->__init == false)
		return 0;
	FD_ZERO(&fdsw);
	FD_ZERO(&fdsr);

	FD_SET(0, &fdsw);
	FD_SET(0, &fdsw);
	FD_SET(this->__sockfd, &fdsw);
	FD_SET(this->__sockfd, &fdsr);
	if (select(this->__sockfd + 1, &fdsr, &fdsw, 0, &timeout) >= 0)
	{
		if (FD_ISSET(this->__sockfd, &fdsw))
			ret = this->write();
		if (FD_ISSET(this->__sockfd, &fdsr))
			ret = this->read();
		if (ret)
			this->quit();
		return 0;
	}
	else
		return 1;
}

buffer*	Client::Get_buf(void)
{
	return &this->__buf_read;
}

bool	Client::Is_run(void)
{
	return this->__init;
}

void		Client::Add_buffer(const char* str, unsigned int size)
{
	this->__buf_write.addstr(str, size);
}

void		Client::quit(void)
{
	if (this->__sockfd != -1)
		close(this->__sockfd);
	freeaddrinfo(this->__res);
	this->__res = 0;
	this->__sockfd = -1;
	this->__init = false;
	this->__buf_write.clear();
	this->__buf_read.clear();
}
