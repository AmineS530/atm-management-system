#include "ui.h"

void init_ui() {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);

    // Force the terminal background to fully render black on start
    bkgd(COLOR_PAIR(5));
    clear();
    refresh();
}

void end_ui() {
    endwin();
}

void draw_box(WINDOW *win, const char *title) {
    int width;
    int height;
    getmaxyx(win, height, width);
    (void)height; // Avoid unused variable warning if not used elsewhere
    box(win, 0, 0);
    if (title) {
        mvwprintw(win, 0, (width - strlen(title)) / 2, " %s ", title);
    }
}

void clear_screen() {
    clear();
    refresh();
}

int show_menu(const char *title, const char **options, int count) {
    int highlight = 0;
    int choice = -1;
    int k;

    int height = count + 4;
    int width = 40;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW *menu_win = newwin(height, width, start_y, start_x);
    keypad(menu_win, TRUE);

    while (1) {
        werase(menu_win);
        wattron(menu_win, COLOR_PAIR(1) | A_BOLD);
        draw_box(menu_win, title);
        wattroff(menu_win, COLOR_PAIR(1) | A_BOLD);

        for (int i = 0; i < count; i++) {
            if (highlight == i) {
                wattron(menu_win, A_REVERSE | COLOR_PAIR(1) | A_BOLD);
                mvwprintw(menu_win, i + 2, 2, "%s", options[i]);
                wattroff(menu_win, A_REVERSE | COLOR_PAIR(1) | A_BOLD);
            } else {
                mvwprintw(menu_win, i + 2, 2, "%s", options[i]);
            }
        }
        wrefresh(menu_win);

        k = wgetch(menu_win);
        switch (k) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = count - 1;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight >= count) highlight = 0;
                break;
            case 10: // Enter
                choice = highlight;
                break;
            case 'q':
            case 'Q':
                choice = -2;
                break;
            default:
                break;
        }
        if (choice != -1) break;
    }

    delwin(menu_win);
    clear_screen();
    return choice;
}

void get_input_field(const char *label, char *buffer, int size, int is_password) {
    int height = 5;
    int width = 50;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW *input_win = newwin(height, width, start_y, start_x);
    wattron(input_win, COLOR_PAIR(1) | A_BOLD);
    draw_box(input_win, label);
    wattroff(input_win, COLOR_PAIR(1) | A_BOLD);
    wrefresh(input_win);
    
    memset(buffer, 0, size);
    
    echo();
    if (is_password) noecho();
    
    curs_set(1);
    mvwgetnstr(input_win, 2, 2, buffer, size - 1);
    curs_set(0);
    noecho();

    delwin(input_win);
    clear_screen();
}

void show_message(const char *message) {
    int height = 5;
    int width = strlen(message) + 10;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW *msg_win = newwin(height, width, start_y, start_x);
    wattron(msg_win, COLOR_PAIR(3) | A_BOLD);
    draw_box(msg_win, "Success");
    mvwprintw(msg_win, 2, 5, "%s", message);
    wattroff(msg_win, COLOR_PAIR(3) | A_BOLD);
    wrefresh(msg_win);
    flushinp();
    wgetch(msg_win);
    delwin(msg_win);
    clear_screen();
}

void show_error(const char *message) {
    int height = 5;
    int width = strlen(message) + 10;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW *err_win = newwin(height, width, start_y, start_x);
    wattron(err_win, COLOR_PAIR(2) | A_BOLD);
    draw_box(err_win, "Error");
    mvwprintw(err_win, 2, 5, "%s", message);
    wattroff(err_win, COLOR_PAIR(2) | A_BOLD);
    wrefresh(err_win);
    flushinp();
    wgetch(err_win);
    delwin(err_win);
    clear_screen();
}
