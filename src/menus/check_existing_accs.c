#include "menus.h"

static void caculate_interest(WINDOW *win, const unsigned char *accountType, double balance, const char *deposit);
static int get_day(const char *deposit_date);

void check_existing_accs(User usr, sqlite3 *db)
{
    if (usr.accCount == 0)
    {
        show_error("No accounts found for user.");
        return;
    }

    int choice = select_account(usr);
    if (choice < 0) return;

    const char *sql = "SELECT accNbr, deposit_date, country, phone, balance, accType "
                      "FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        show_error("Database error.");
        return;
    }

    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int64(stmt, 2, usr.accountIds[choice]);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        WINDOW *info_win = newwin(15, 60, (LINES - 15) / 2, (COLS - 60) / 2);
        draw_box(info_win, "Account Details");

        wattron(info_win, A_BOLD | COLOR_PAIR(1));
        mvwprintw(info_win, 2, 2, "Account Number:");
        mvwprintw(info_win, 3, 2, "Deposit Date:  ");
        mvwprintw(info_win, 4, 2, "Country:       ");
        mvwprintw(info_win, 5, 2, "Phone:         ");
        mvwprintw(info_win, 6, 2, "Balance:       ");
        mvwprintw(info_win, 7, 2, "Account Type:  ");
        wattroff(info_win, A_BOLD | COLOR_PAIR(1));

        mvwprintw(info_win, 2, 18, "%lld", (long long)sqlite3_column_int64(stmt, 0));
        mvwprintw(info_win, 3, 18, "%s", sqlite3_column_text(stmt, 1));
        mvwprintw(info_win, 4, 18, "%s", sqlite3_column_text(stmt, 2));
        mvwprintw(info_win, 5, 18, "%s", sqlite3_column_text(stmt, 3));
        mvwprintw(info_win, 6, 18, "$%.2f", sqlite3_column_double(stmt, 4));
        mvwprintw(info_win, 7, 18, "%s", sqlite3_column_text(stmt, 5));

        caculate_interest(info_win, sqlite3_column_text(stmt, 5), 
                         sqlite3_column_double(stmt, 4), 
                         (const char *)sqlite3_column_text(stmt, 1));

        mvwprintw(info_win, 13, 2, "Press any key to return...");
        wrefresh(info_win);
        wgetch(info_win);
        delwin(info_win);
    }

    sqlite3_finalize(stmt);
    clear_screen();
}

static void caculate_interest(WINDOW *win, const unsigned char *accountType, double balance, const char *deposit)
{
    if (strcmp((const char *)accountType, "Current") == 0)
    {
        mvwprintw(win, 9, 2, "Interest: No interest for Current accounts.");
    }
    else
    {
        double interestRate = 0.0;
        int years = 0;
        if (strcmp((const char *)accountType, "Savings") == 0) interestRate = 0.07;
        else if (strcmp((const char *)accountType, "Fixed01") == 0) { interestRate = 0.04; years = 1; }
        else if (strcmp((const char *)accountType, "Fixed02") == 0) { interestRate = 0.05; years = 2; }
        else if (strcmp((const char *)accountType, "Fixed03") == 0) { interestRate = 0.08; years = 3; }
            
        wattron(win, A_BOLD | COLOR_PAIR(4));
        mvwprintw(win, 9, 2, "Interest info:");
        wattroff(win, A_BOLD | COLOR_PAIR(4));
        
        if (years == 0) { // Savings
            double interest = (balance * interestRate) / 12.0;
            mvwprintw(win, 10, 4, "You will get ");
            wattron(win, A_BOLD | COLOR_PAIR(3));
            wprintw(win, "$%.2f", interest);
            wattroff(win, A_BOLD | COLOR_PAIR(3));
            wprintw(win, " as interest on day %d of every month.", get_day(deposit));
        } else {
            double total_interest = balance * interestRate * years;
            int day, month, year;
            sscanf(deposit, "%d/%d/%d", &day, &month, &year);
            mvwprintw(win, 10, 4, "You will get ");
            wattron(win, A_BOLD | COLOR_PAIR(3));
            wprintw(win, "$%.2f", total_interest);
            wattroff(win, A_BOLD | COLOR_PAIR(3));
            wprintw(win, " as interest on %02d/%02d/%04d.", day, month, year + years);
        }
    }
}

static int get_day(const char *deposit_date)
{
    int day = 0;
    sscanf((const char *)deposit_date, "%d/", &day);
    return day;
}