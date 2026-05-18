#ifndef UI_H
#define UI_H

#include "ncurses.h"
#include <string.h>
#include <stdlib.h>
#include <locale.h>

typedef struct {
    int start_y, start_x;
    int height, width;
    WINDOW *win;
} UI_Window;

void init_ui();
void end_ui();
void draw_box(WINDOW *win, const char *title);
void clear_screen();
int show_menu(const char *title, const char **options, int count);
void get_input_field(const char *label, char *buffer, int size, int is_password);
void show_message(const char *message);
void show_error(const char *message);

#endif
