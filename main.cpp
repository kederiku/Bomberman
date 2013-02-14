#include "window.h"
#include "menu.h"
#include "callback.h"
#include "game.h"
#include "network_game.h"
#include <iostream>

Window*	Window::__instance = 0;

bool	init_credit(Window* screen)
{
	Interface*		credit;

	credit = new (std::nothrow) Interface();
	if (credit == 0 || credit->Init())
	{
		delete credit;
		return 1;
	}
	if (credit->Add_text("MEYER Benjamin", 300, 175, 150, 150, 150))
	{
		delete credit;
		return 1;
	}
	if (credit->Add_text("DELAGREE Cedric", 300, 275, 150, 150, 150))
	{
		delete credit;
		return 1;
	}
	if (credit->Add_text("HUYNH Yves", 300, 375, 150, 150, 150))
	{
		delete credit;
		return 1;
	}
	if (screen->Add_screen(credit, CREDIT))
		return 1;
	return 0;
}

bool	init_menu(Window* screen)
{
	Menu*		menu;

	menu = new (std::nothrow) Menu();
	if (menu == 0 || menu->Init(75, 220))
	{
		delete menu;
		return 1;
	}
	if (menu->Add_items("Network Game", change_screen<NETWORK_GAME>, 0) ||
	    menu->Add_items("Local Game", change_screen<LOCAL_GAME>, 0) ||
	    menu->Add_items("Options", change_screen<OPTION>, 0) ||
	    menu->Add_items("Credit", change_screen<CREDIT>, 0) ||
	    menu->Add_items("Quit", quit, 0) || menu->Add_img("img/title.bmp", 475, 0))
	{
		delete menu;
		return 1;
	}
	if (screen->Add_screen(menu, MAIN))
		return 1;
	return 0;
}

bool	init_game(Window* screen)
{
	Game*		game;

	game = new (std::nothrow) Game();
	if (game == 0 || game->Init(2) || screen->Add_screen(game, LOCAL_GAME))
	{
		delete game;
		return 1;
	}
	return 0;
}

bool	init_game_over(Window* screen)
{
	Menu*		menu;
	Interface*	text;

	text = new (std::nothrow) Interface();
	if (text == 0 || text->Init() || text->Add_text("GAME OVER", 300, 50, 15, 150, 150))
	{
		delete text;
		return 1;
	}
	menu = new (std::nothrow) Menu();
	if (menu == 0 || menu->Init(75, 220, 125, 200) ||
	    menu->Add_items("New Game", change_screen<LOCAL_GAME>, 0) ||
	    menu->Add_items("Main Menu", change_screen<MAIN>, 0) ||
	    menu->Add_items("Quit", quit, 0) || text->Add_img("img/game_over.bmp", 300, 80) || text->Add_child(menu))
	{
		delete text;
		delete menu;
		return 1;
	}
	if (screen->Add_screen(text, GAME_OVER))
		return 1;
	return 0;
}

bool	init_option(Window* screen)
{
	Menu*		menu;

	menu = new (std::nothrow) Menu();
	if (menu == 0 || menu->Init(75, 220))
	{
		delete menu;
		return 1;
	}
	if (menu->Add_items("Sound", fct, 0))
	{
		delete menu;
		return 1;
	}
	if (menu->Add_items("Keyboard", fct, 0))
	{
		delete menu;
		return 1;
	}
	if (screen->Add_screen(menu, OPTION))
		return 1;
	return 0;
}

bool	init_window(Window* screen)
{
	if (init_network_game(screen))
		return 1;
	if (init_credit(screen))
		return 1;
	if (init_game(screen))
		return 1;
	if (init_game_over(screen))
		return 1;
	if (init_option(screen))
		return 1;
	if (init_menu(screen))
		return 1;
	return 0;
}

int	main(void)
{
	Window*	screen;

	screen = Window::GetInstance(800, 600);
	if (screen == 0)
		return 1;
	if (init_window(screen))
	{
		Window::Kill();
		return 1;
	}
	screen->Wait_event();
	Window::Kill();
	return (0);
}
