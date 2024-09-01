CC = gcc

APP = ATM

SRC = src/main.c \
	src/system.c \
	src/auth.c \
	src/errors.c

OBJ = $(SRC:.c=.o)

%.o: src/atm_sys.h

all : $(APP)

$(APP) : $(OBJ) src/atm_sys.h
	@echo "\033[1;38;5;155mBuilding $@... \033[0m"
	@$(CC) -o $@.exec $(OBJ)
clean :
	@echo "\033[1;38;5;196mDeleting object files from the directory...\033[0m"
	@rm -f $(OBJ)

fclean : clean
	@echo "\033[1;38;5;196mRemoving The ATM Managment System from path...\033[0m"
	@rm $(APP)

re : clean all

.PHONY: all clean fclean