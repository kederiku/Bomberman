#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "interface.h"
#include "server.h"
#include "client.h"
#include "bomberman.h"
#include <pthread.h>
#include <SDL/SDL_ttf.h>

class	Window: public Control
{
	bool			__run;
	static	Window*		__instance;
	TTF_Font*		__font;
	Interface*		__actif;
	server			__network;
	Client			__client;
	Bomberman		__game;
	pthread_t		__threads[2];
	bool			__server_statut;
	pthread_mutex_t		__mutex;

	Window(void);
	virtual ~Window(void);

	virtual int		Get_escape(void);
	bool			Launch_callback(SDL_Event* event);
	bool			Init(int w, int h);
	virtual Control*	Get_control(int x, int y);
	virtual bool		Read_client(void);
public:
	inline static Window*	GetInstance(void)
	{
		return __instance;
	};
	inline static Window*	GetInstance(int w, int h)
	{
		if (__instance == 0)
			__instance = new (std::nothrow) Window;
		if (__instance != 0 && __instance->Init(w, h))
			Kill();
		return __instance;
	};
	static void		Kill(void)
	{
		delete __instance;
		__instance = 0;
	};
	TTF_Font*	Get_font(void) const;
	SDL_Surface*	Get_surface(void) const;
	bool		Add_screen(Interface* child, id_screen id, id_screen prev = MAIN);
	bool		Change_screen(id_screen id);
	void		Wait_event(void);
	server*		Get_server(void);
	Client*		Get_client(void);
	Bomberman*	Get_game(void);
	bool		Is_run(void);
	bool		Get_server_statut(void);
	void		Set_server_statut(bool statut);
};

#endif
