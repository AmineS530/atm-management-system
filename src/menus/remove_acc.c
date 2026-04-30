#include "menus.h"

void remove_acc(sqlite3 *db, User *usr)
{
    if (usr->accCount == 0)
    {
        show_error("No accounts found.");
        return;
    }

    int choice = select_account(*usr);
    if (choice < 0) return;

    const char *options[] = {"No, Cancel", "Yes, Delete"};
    char title[100];
    snprintf(title, sizeof(title), "Are you sure you want to delete account %ld?", usr->accountIds[choice]);
    int confirm = show_menu(title, options, 2);

    if (confirm != 1)
    {
        return;
    }

    const char *sql = "DELETE FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        show_error("Database error.");
        return;
    }

    sqlite3_bind_int(stmt, 1, usr->id);
    sqlite3_bind_int64(stmt, 2, usr->accountIds[choice]);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        show_error("Failed to remove account.");
    } else {
        show_message("Account removed successfully.");
    }
    sqlite3_finalize(stmt);
    get_acc_nbrs(usr, db);
}