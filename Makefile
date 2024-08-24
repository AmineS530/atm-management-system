CC = gcc

APP = atm

SRC = src/main.c \
	src/system.c \
	src/auth.c

OBJ = $(SRC:.c=.o)

all : atm
	
%.o: src/header.h

$(APP) : $(OBJ) src/header.h
	@echo "\033[1;38;5;155mBuilding $@... \033[0m"
	@$(CC) -o atm $(OBJ)

clean :
	@echo "\033[1;38;5;196mDeleting object files from the directory...\033[0m"
	@rm -f $(OBJ)

fclean : clean
	@echo "\033[1;38;5;196mRemoving The ATM Managment System from path...\033[0m"
	@rm $(APP)

re : fclean all

.PHONY: all clean fclean