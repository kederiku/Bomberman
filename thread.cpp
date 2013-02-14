#include "thread.h"
#include "window.h"
#include "server.h"
#include "client.h"
#include <iostream>

void*	id_thread(void* data)
{
	Window*		screen;
	server*		network;
	fd_set		fdsr;
	fd_set		fdsw;
	int		fdmax;
	struct timeval	timeout;

	screen = (Window*)data;
	network = screen->Get_server();
	timeout.tv_sec = 0;
	timeout.tv_usec = 500;
	while (screen->Is_run())
	{

		network->run(&fdsr, &fdsw, &fdmax, &timeout);
		timeout.tv_usec = 500;
		if (screen->Get_server_statut())
		{
			network->quit();
			screen->Set_server_statut(false);
		}
		usleep(1);
	}
	return 0;
}

void*	thread_client(void* data)
{
	Window*		screen;
	Client*		network;

	screen = (Window*)data;
	network = screen->Get_client();
	while (screen->Is_run())
	{
		network->run();
		usleep(1);
	}

	return 0;
}
