NAME=philo

C_FILES= ft_aux.c\
		 process.c\
		 main.c\


CC=cc
CFLAGS=-Wall -Wextra -Werror -pthread  -g
REMOVE = rm -rf

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(C_FILES) -o $(NAME)

clean:
	rm -f $(O_FILES)

fclean: clean
	$(REMOVE) $(NAME)

re: fclean all	

.PHONY: all clean fclean re