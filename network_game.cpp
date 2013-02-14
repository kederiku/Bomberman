#include "menu.h"
#include "callback.h"
#include "window.h"
#include "text_area.h"
#include "text_box.h"
#include "waiting_room.h"
#include "game_online.h"

bool	init_game_online(Window* screen)
{
	Game_online*	game;

	game = new (std::nothrow) Game_online();
	if (game == 0 || game->Init(0, 0))
	{
		delete game;
		return 1;
	}
	if (screen->Add_screen(game, GAME_ONLINE, NETWORK_GAME))
		return 1;
	return 0;
}

bool	init_wait_player(Window* screen)
{
	Waiting_room*	wait_player;

	wait_player = new (std::nothrow) Waiting_room();
	if (wait_player == 0 || wait_player->Init(0, 0))
	{
		delete wait_player;
		return 1;
	}
	if (wait_player->Add_text("WAIT PLAYER", 300, 100, 150, 150, 150))
	{
		delete wait_player;
		return 1;
	}
	if (screen->Add_screen(wait_player, WAIT_PLAYER, NETWORK_GAME))
		return 1;
	return 0;
}

bool	init_create_game(Window* screen)
{
	Text_box*	port;

	port = new (std::nothrow) Text_box;
	if (port == 0 || port->Init(100, 100, 400, 300, CREATE) ||
	    port->Add_text_area(0, 50, 400, 26, "Port: "))
	{
		delete port;
		return 1;
	}
	if (screen->Add_screen(port, CREATE_GAME, NETWORK_GAME))
		return 1;
	return 0;
}

bool	init_join_game(Window* screen)
{
	Text_box*	join;

	join = new (std::nothrow) Text_box;
	if (join == 0 || join->Init(100, 100, 400, 300, JOIN) ||
	    join->Add_text_area(0, 0, 400, 26, "Adress: ") ||
	    join->Add_text_area(0, 50, 400, 26, "Port: "))
	{
		delete join;
		return 1;
	}
	if (screen->Add_screen(join, JOIN_GAME, NETWORK_GAME))
		return 1;
	return 0;
}

bool	init_network_game(Window* screen)
{
	Menu*	menu;

	if (init_game_online(screen))
		return 1;
	if (init_join_game(screen))
		return 1;
	if (init_wait_player(screen))
		return 1;
	if (init_create_game(screen))
		return 1;
	menu = new (std::nothrow) Menu();
	if (menu == 0 || menu->Init(75, 220))
	{
		delete menu;
		return 1;
	}
	if (menu->Add_items("Create Game", change_screen<CREATE_GAME>, 0) ||
	    menu->Add_items("Join Game", change_screen<JOIN_GAME>, 0))
	{
		delete menu;
		return 1;
	}
	if (screen->Add_screen(menu, NETWORK_GAME))
		return 1;
	return 0;
}
