CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = ncurses

AR = ar rcs

VERSION = 6.5
ARCHIVE = ncurses-$(VERSION).tar.gz
URL = https://ftp.gnu.org/gnu/ncurses/$(ARCHIVE)
DIR = ncurses-$(VERSION)

LIB = libncurses.a

all: getNcurses build

getNcurses:
	@if [ ! -f "$(LIB)" ]; then \
		echo "\033[1;38;5;155mDownloading Ncurses...\033[0m"; \
		wget $(URL) -q && \
		tar -xzf $(ARCHIVE) && \
		rm -f $(ARCHIVE); \
	fi

build:
	@if [ ! -f "$(LIB)" ]; then \
		echo "\033[1;38;5;155mBuilding Ncurses locally...\033[0m"; \
		(cd $(DIR) && \
		./configure --without-shared --enable-static --prefix=$$(pwd)/build --with-default-terminfo-dir=/usr/share/terminfo --with-terminfo-dirs=/etc/terminfo:/lib/terminfo:/usr/share/terminfo > /dev/null && \
		make -j$$(nproc) > /dev/null && \
		(cp lib/libncurses.a ../$(LIB) 2>/dev/null || cp lib/libncursesw.a ../$(LIB)) && \
		cp include/*.h ../) && \
		rm -rf $(DIR); \
		echo "\033[1;38;5;155m$(LIB) Created\033[0m"; \
	fi

clean:
	@if [ -d "$(DIR)" ] && [ -f "$(DIR)/Makefile" ]; then \
		cd $(DIR) && make clean > /dev/null 2>&1 || true; \
	fi

fclean: clean
	@rm -rf $(DIR)
	@rm -f $(LIB)
	@rm -f curses.h ncurses.h term.h

re: fclean all

.PHONY: all getNcurses build clean fclean re