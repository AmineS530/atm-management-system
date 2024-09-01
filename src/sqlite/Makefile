CC = cc

CFLAGS = -c

NAME = libsqlite3.a

AR = ar rcs

CFILES = shell.c \
		sqlite3.c \

OBJ = $(CFILES:.c=.o)


all: $(NAME)

%.o : %.c sqlite3.h
	@$(CC) $(CFLAGS) $< -o $@

$(NAME): $(OBJ) sqlite3.h
	$(AR)  $@ $^

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
