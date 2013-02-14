NAME		=	id_bomberman

SRC		=	bomb.cpp	\
			bomberman.cpp	\
			buffer.cpp	\
			bonus.cpp	\
			button.cpp	\
			client.cpp	\
			callback.cpp	\
			control.cpp	\
			game.cpp	\
			game_online.cpp	\
			event.cpp	\
			label.cpp	\
			main.cpp	\
			map.cpp		\
			menu.cpp	\
			network_game.cpp\
			player.cpp	\
			info.cpp	\
			image.cpp	\
			interface.cpp	\
			items.cpp	\
			server.cpp	\
			string.cpp	\
			timer.cpp	\
			text_area.cpp	\
			text_box.cpp	\
			thread.cpp	\
			user.cpp	\
			window.cpp	\
			waiting_room.cpp

CFLAGS		+=	-W -Wall -Werror -g3

LDFLAGS		=	-lSDL -lSDL_ttf -lpthread

%.o: %.cpp
			g++ $(CFLAGS) -o $@ -c $<

OBJ		=	$(SRC:.cpp=.o)

$(NAME)		:	$(OBJ)
			g++ -o $(NAME) $(OBJ) $(CFLAGS) $(LDFLAGS)
clean		:
			rm -rf $(OBJ)

distclean	:	clean
			rm -rf $(NAME)

rebuild		:	distclean all

all		:	$(NAME)
