NAME = pipex
LIBRARY = libft/libft.a
CC = cc
FLAGS = -Wall -Wextra -Werror
OBJECTS = closing.o main.o manage_cmd.o manage_input_src.o manage_output_dst.o utilities.o

.PHONY: all clean fclean re

all: $(NAME)

$(LIBRARY):
	$(MAKE) -C ./libft

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(LIBRARY) $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) $(LIBRARY) -o $(NAME)

clean:
	$(MAKE) -C ./libft clean
	rm -f $(OBJECTS)

fclean: clean
	$(MAKE) -C ./libft fclean
	rm -f $(NAME)

re: fclean all