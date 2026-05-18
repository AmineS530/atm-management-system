CC = gcc

APP = z1_ATM

CFLAGS = -g -Wall -Wextra 

LDFLAGS = src/ncurses/libncurses.a

SRC = src/main.c \
	src/auth/auth.c \
	src/auth/encrypt.c \
	src/helpers/errors.c \
	src/helpers/utils.c \
	src/input/input_utils.c \
	src/helpers/init.c \
	src/helpers/logs.c \
	src/helpers/query_helpers.c \
	src/menus/menus.c \
	src/menus/create_account.c \
	src/menus/check_existing_accs.c \
	src/menus/check_all_accs.c \
	src/menus/make_transaction.c \
	src/menus/remove_acc.c \
	src/menus/transfer_ownership.c \
	src/menus/update_acc.c \
	src/ui/ui.c

SQLITE3_PATH = src/sqlite
NCURSES_PATH = src/ncurses

OBJ = $(SRC:.c=.o)

%.o: %.c src/atm_sys.h
	@$(CC) $(CFLAGS) -I$(NCURSES_PATH) -c $< -o $@

all: sqlite ncurses $(APP)

sqlite:
	@if [ ! -f "./$(SQLITE3_PATH)/libsqlite3.a" ]; then \
		make -C $(SQLITE3_PATH) --silent ;\
	else \
		echo "\033[1;38;5;214mlibsqlite3.a already exist, skipping library build... \033[0m" ; \
	fi

ncurses:
	@if [ ! -f "./$(NCURSES_PATH)/libncurses.a" ]; then \
		make -C $(NCURSES_PATH) --silent ;\
	else \
		echo "\033[1;38;5;214mlibncurses.a already exist, skipping library build... \033[0m" ; \
	fi

$(APP): $(OBJ)
	@echo "\033[1;38;5;155mBuilding $@... \033[0m"
	@$(CC) $(CFLAGS) -o $@.exec $(OBJ) $(SQLITE3_PATH)/libsqlite3.a $(LDFLAGS)
	@echo "\033[1;38;5;155m$@.exec Created\033[0m"

clean:
	@echo "\033[1;38;5;196mDeleting object files from the directory...\033[0m"
	@rm -fr $(OBJ) $(DEP)
	@make -C $(SQLITE3_PATH) clean --silent
	@make -C $(NCURSES_PATH) clean --silent

fclean: clean
	@echo "\033[1;38;5;196mRemoving The ATM Management System from path...\033[0m"
	@rm -fr $(APP).exec

re: fclean all

rea: fclean all
	@make -C $(SQLITE3_PATH) fclean --silent
	@make -C $(NCURSES_PATH) fclean --silent

.PHONY: clean fclean re rea