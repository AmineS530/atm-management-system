CC = gcc

APP = ATM

SRC = src/main.c \
	src/system.c \
	src/auth.c \
	src/errors.c \
	src/todo.c

OBJ = $(SRC:.c=.o)

SQLITE3_PATH = src/sqlite/

all: sqlite $(APP)

sqlite:
	@make -C $(SQLITE3_PATH) --silent

$(APP): $(OBJ)
	@echo "\033[1;38;5;155mBuilding $@... \033[0m"
	@$(CC) $(CFLAGS) -o $@.exec $(OBJ) $(SQLITE3_PATH)/libsqlite3.a

clean:
	@echo "\033[1;38;5;196mDeleting object files from the directory...\033[0m"
	@rm -fr $(OBJ)
	@make -C $(SQLITE3_PATH) clean --silent

fclean: clean
	@echo "\033[1;38;5;196mRemoving The ATM Management System from path...\033[0m"
	@rm -fr $(APP).exec
	@make -C $(SQLITE3_PATH) fclean --silent

re: fclean all

.PHONY: clean fclean re
