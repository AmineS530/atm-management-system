CC = gcc

APP = ATM

CFLAGS = -Wall -Wextra

SRC = src/main.c \
	src/menus.c \
	src/system.c \
	src/auth.c \
	src/errors.c \
	src/encrypt.c \
	src/utils.c \
	src/input_utils.c \
	src/todo.c

SQLITE3_PATH = src/sqlite

OBJ = $(SRC:.c=.o)

%.o: %.c src/atm_sys.h
	$(CC) $(CFLAGS) -c $< -o $@

all: sqlite $(APP)

sqlite:
	@if [ ! -f "./$(SQLITE3_PATH)/libsqlite3.a" ]; then \
		make -C $(SQLITE3_PATH) --silent ;\
	else \
		echo "\033[1;38;5;214mlibsqlite3.a already exist, skipping library build... \033[0m" ; \
	fi

$(APP): $(OBJ)
	@echo "\033[1;38;5;155mBuilding $@... \033[0m"
	$(CC) $(CFLAGS) -o $@.exec $(OBJ) $(SQLITE3_PATH)/libsqlite3.a

clean:
	@echo "\033[1;38;5;196mDeleting object files from the directory...\033[0m"
	@rm -fr $(OBJ) $(DEP)
	@make -C $(SQLITE3_PATH) clean --silent

fclean: clean
	@echo "\033[1;38;5;196mRemoving The ATM Management System from path...\033[0m"
	@make -C $(SQLITE3_PATH) fclean --silent
	@rm -fr $(APP).exec

re: fclean all

.PHONY: clean fclean re
