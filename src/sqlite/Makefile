CC = cc

CFLAGS = -c

NAME = sqlite3

AR = ar rcs

CFILES = shell.c \
		sqlite3.c \

OBJ = $(CFILES:.c=.o)


all: getSqlite 
	@if [ ! -f "libsqlite3.a" ]; then \
		make $(NAME) ;\
	else \
		echo "\033[1;38;5;214mlibsqlite3.a already exist, skipping library build... \033[0m" ; \
	fi

%.o : %.c sqlite3.h
	@$(CC) $(CFLAGS) $< -o $@ 
	
$(NAME): $(OBJ) sqlite3.h 
	@echo "\033[1;38;5;155mBuilding $@ library... \033[0m"
	$(AR)  lib$@.a $^
	@echo "\033[1;38;5;155mlibsqlite.a Created \033[0m"

getSqlite:
	@if [ ! -f "libsqlite3.a" ]; then \
		wget https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip 2>/dev/null && \
		unzip sqlite-amalgamation-3460100.zip >/dev/null && \
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
