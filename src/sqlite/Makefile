CC = cc

CFLAGS = -Wall -Werror

NAME = sqlite3

AR = ar rcs

CFILES = shell.c \
		sqlite3.c \

OBJ = $(CFILES:%.c=%.o)

all: getSqlite 
	@if [ ! -f "libsqlite3.a" ]; then \
		echo "\033[1;38;5;155mBuilding $(NAME) library... \033[0m" && \
		make $(NAME) ;\
	fi

%.o : %.c sqlite3.h
	@$(CC) $(CFLAGS) -c $< -o $@ 
	
$(NAME): $(OBJ)
	$(AR)  lib$@.a $^
	@echo "\033[1;38;5;155mlibsqlite.a Created \033[0m"

getSqlite:
	@if [ ! -f "libsqlite3.a" ]; then \
		wget https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip 2>/dev/null && \
		unzip -qq sqlite-amalgamation-3460100.zip && \
		cd sqlite-amalgamation-3460100 && mv * ../ && cd .. && \
		rm -fr sqlite-amalgamation-3460100.zip sqlite-amalgamation-3460100 ; \
	fi

clean:
	@rm -rf $(OBJ)
	@rm -rf $(CFILES)

fclean: clean
	@rm -rf lib$(NAME).a

re: fclean all

.PHONY: clean fclean re
