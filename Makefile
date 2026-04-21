CC      = gcc
FLAGS   = -Wall -Wextra -pthread
LIBS    = -lrt
SRC     = main.c timer.c socket.c logger.c file_writer.c daemon.c
OBJ     = $(SRC:.c=.o)
NAME    = sysmon

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re