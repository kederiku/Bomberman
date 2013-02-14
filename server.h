#ifndef _SERVER_H_
#define _SERVER_H_

#include "timer.h"
#include "response.h"
#include "bomberman.h"
#include <list>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class	user;

class	server
{
	struct addrinfo*	__res;
	int			__sockfd;
	std::list<user*>	__client;
	bool			__init;
	Bomberman		__game;
	bool			__game_ready;

	bool	Add_client(void);
	int	Treat_client(user* client);
	void	Init_fd(fd_set* fdsr, fd_set* fdsw, int* fdmax);
	bool	Check_fd(fd_set* fdsr, fd_set* fdsw);
	bool	treat_client(user* player);
	void	Init(void);
	void	Clear(void);
	void	Send_all(void);
	void	Send_map(void);
	void	Send_player(unsigned int, unsigned int, char);
	void	Send_item(void);
	void	Send_nb_player();


	void	Send_user(void* data, unsigned int size);
public:
	server(void);
	~server(void);

	void			quit(void);
	bool			Is_run(void);
	bool			run(fd_set* fdsr, fd_set* fdsw, int* fdmax, struct timeval* timeout);
	bool			Init(char* port);
	std::list<user*>*	Get_user(void);
	bool			Init_game(void);
	Bomberman*		Get_game(void);
};

#endif
