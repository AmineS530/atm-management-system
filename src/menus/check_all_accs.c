#include "menus.h"

void check_all_accounts(User usr, sqlite3 *db)
{
    if (usr.accCount == 0)
    {
        show_error("No accounts found.");
        return;
    }

    const char *sql = "SELECT accNbr, deposit_date, country, phone, balance, accType FROM records WHERE userID = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        show_error("Database error.");
        return;
    }

    sqlite3_bind_int(stmt, 1, usr.id);

    WINDOW *list_win = newwin(20, 80, (LINES - 20) / 2, (COLS - 80) / 2);
    draw_box(list_win, "All Accounts");
    
    // Header
    wattron(list_win, A_BOLD);
    mvwprintw(list_win, 2, 2, "%-12s | %-10s | %-10s | %-10s | %-10s", 
              "Account #", "Country", "Type", "Balance", "Date");
    mvwhline(list_win, 3, 2, ACS_HLINE, 76);
    wattroff(list_win, A_BOLD);

    int line = 4;
    while (sqlite3_step(stmt) == SQLITE_ROW && line < 18)
    {
        const char *country = (const char *)sqlite3_column_text(stmt, 2);
        const char *type = (const char *)sqlite3_column_text(stmt, 5);
        const char *date = (const char *)sqlite3_column_text(stmt, 1);

        mvwprintw(list_win, line++, 2, "%-12lld | %-10s | %-10s | $%-9.2f | %-10s",
                 (long long)sqlite3_column_int64(stmt, 0),
                 country ? country : "N/A",
                 type ? type : "N/A",
                 sqlite3_column_double(stmt, 4),
                 date ? date : "N/A");
    }
    
    mvwprintw(list_win, 18, 2, "Press any key to return...");
    wrefresh(list_win);
    wgetch(list_win);
    delwin(list_win);

    sqlite3_finalize(stmt);
    clear_screen();
}
