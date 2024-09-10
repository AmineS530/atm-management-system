CC = cc

CFLAGS = -c

NAME = sqlite3

AR = ar rcs

CFILES = shell.c \
		sqlite3.c \

OBJ = $(CFILES:.c=.o)


all: getSqlite $(NAME)

getSqlite:
	@wget https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip 2>/dev/null
	@unzip sqlite-amalgamation-3460100.zip
	@cd sqlite-amalgamation-3460100 && mv * ../
	@rm -fr sqlite-amalgamation-3460100.zip && rm -fr sqlite-amalgamation-3460100 2>/dev/null

%.o : %.c sqlite3.h
	@$(CC) $(CFLAGS) $< -o $@
	
$(NAME): $(OBJ) sqlite3.h
	@echo "\033[1;38;5;155mBuilding $@ library... \033[0m"
	$(AR)  lib$@.a $^

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf lib$(NAME).a

re: fclean all

.PHONY: all clean fclean re
