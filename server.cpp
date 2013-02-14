#include "server.h"
#include "user.h"
#include "string.h"
#include "response.h"
#include "window.h"
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

server::server(void)
{
	this->Init();
}

server::~server(void)
{
	this->Clear();
}

bool	server::Init(char* port)
{
	struct	addrinfo	hints;

	if (this->__init == true)
		return 0;
	id_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(0, port, &hints, &this->__res) != 0)
		return 1;
	this->__sockfd = socket(this->__res->ai_family, this->__res->ai_socktype,
			        this->__res->ai_protocol);
	if (this->__sockfd == -1)
		return 1;
	if (bind(this->__sockfd, this->__res->ai_addr, this->__res->ai_addrlen) == -1)
		return 1;
	if (listen(this->__sockfd, 10) == -1)
		return 1;
	this->__init = true;
	return 0;
}

void	server::Init(void)
{
	this->__res = 0;
	this->__sockfd = -1;
	this->__init = false;
	this->__game_ready = false;
}

void	server::Clear(void)
{
	std::list<user*>::iterator	it;

	it = this->__client.begin();
	while (it != this->__client.end())
	{
		delete *it;
		++it;
	}
	this->__client.clear();
	freeaddrinfo(this->__res);
	if (this->__sockfd != -1)
		close(this->__sockfd);
}

bool	server::Add_client(void)
{
	user*	client;

	try
	{
		client = new user();
		if (client->Init(this->__sockfd))
		{
			delete client;
			return true;
		}
		this->__client.push_back(client);
	}
	catch (std::exception ex)
	{
		delete client;
		return true;
	}
	return false;
}

void	server::Init_fd(fd_set* fdsr, fd_set* fdsw, int* fdmax)
{
	std::list<user*>::iterator	it;

	FD_ZERO(fdsw);
	FD_ZERO(fdsr);
	FD_SET(this->__sockfd, fdsr);
	it = this->__client.begin();
	while (it != this->__client.end())
	{
			FD_SET((*it)->Get_fd(), fdsw);
			FD_SET((*it)->Get_fd(), fdsr);
		if ((*it)->Get_fd() + 1 > *fdmax)
			*fdmax = (*it)->Get_fd() + 1;
		++it;
	}
}

bool	server::Check_fd(fd_set* fdsr, fd_set* fdsw)
{
	std::list<user*>::iterator	it;
	bool				ret;

	it = this->__client.begin();
	while (it != this->__client.end())
	{
		ret = 0;
		if (FD_ISSET((*it)->Get_fd(), fdsr))
			ret = (*it)->read();
		if (FD_ISSET((*it)->Get_fd(), fdsw))
			ret = (*it)->write();
		if (ret || this->treat_client(*it) == 1)
		{
			delete *it;
			this->__client.erase(it--);
			this->Send_nb_player();
		}
		++it;
	}
	return false;
}

bool	server::run(fd_set* fdsr, fd_set* fdsw, int* fdmax, struct timeval* timeout)
{
	if (this->__init == false)
		return 0;
	*fdmax = this->__sockfd + 1;
	this->Init_fd(fdsr, fdsw, fdmax);
	if (select(*fdmax, fdsr, fdsw, 0, timeout) >= 0)
	{
		if (FD_ISSET(this->__sockfd, fdsr))
		{
			if (this->Add_client())
				return 1;
			this->Send_nb_player();
		}
		this->Check_fd(fdsr, fdsw);
		this->__game.Search_explosion();
		return 0;
	}
	else
		return 1;
}

std::list<user*>*	server::Get_user(void)
{
	return &this->__client;
}


bool	server::Is_run(void)
{
	return this->__init;
}

bool	server::treat_client(user* player)
{
	buffer*			buf;
	Client_response*	data;
	int			id;
	Position		pos;

	id = player->Get_id();
	if (id == -1)
		return 0;
	buf = player->get_buf();
	if (buf->size() < sizeof(Client_response))
		return 0;
	data = (Client_response*)buf->get_str();
	this->__game.Move_player(id, data->move, data->bomb, &pos);
	if (pos.x != 0 && pos.y != 0)
		this->Send_all();
	buf->remove(sizeof(Client_response));
	return 0;
}

void	server::quit(void)
{
	this->Clear();
	this->Init();
}

bool	server::Init_game(void)
{
	std::list<user*>::iterator	it;
	int				i;

	if (this->__game_ready)
		return 0;
	it = this->__client.begin();
	i = 0;
	while (it != this->__client.end())
	{
		(*it)->Set_id(i);
		++it;
		++i;
	}
	this->__game_ready = true;
	this->Send_nb_player();
	this->Send_all();
	return 0;
}

Bomberman*	server::Get_game(void)
{
	return &this->__game;
}

void	server::Send_user(void* data, unsigned int size)
{
	std::list<user*>::iterator	it;

	it = this->__client.begin();
	while (it != this->__client.end())
	{
		(*it)->Add_buffer((const char*)data, size);
		++it;
	}
}

void	server::Send_all(void)
{
	Server_response	resp;

	if (this->__client.size() < 9)
		resp.nb_player = this->__client.size();
	resp.game_ready = this->__game_ready;
	this->__game.Init_Responseserver(&resp);
	this->Send_user(&resp, resp.info.size);
}

void	server::Send_map(void)
{
	Map_change	resp;

	this->Send_user(&resp, resp.info.size);
}


void	server::Send_player(unsigned int x, unsigned int y, char id)
{
	Player_change	resp;

	resp.pos.x = x;
	resp.pos.y = y;
	resp.id = id;
	this->Send_user(&resp, resp.info.size);
}


void	server::Send_item(void)
{
	Item_change	resp;

	this->Send_user(&resp, resp.info.size);
}

void	server::Send_nb_player(void)
{
	Nb_player	resp;

	if (this->__client.size() < 9)
		resp.nb_player = this->__client.size();
	resp.game_ready = this->__game_ready;
	this->Send_user(&resp, resp.info.size);
}
