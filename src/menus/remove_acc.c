#include "../atm_sys.h"

// **Remove existing account** function
void remove_acc(sqlite3 *db, User *usr)
{
    system("clear");
    printf("\t\t====== Remove Account ======\n\n");
    if (usr->accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr->name);
        return;
    }
    system("clear");
    printf("\t\t====== Remove Account ======\n\n");
    int choice = select_account(*usr);
    printf("\t\t====== Remove Account ======\n\n");
    printf("\t\tselected account number: %ld\n", usr->accountIds[choice]);
    printf("\n\t\tAre you sure you want to delete this account?\n\t\t"
           "this action cannot be undone.(y/n): ");
    char c = confirm_char();

    if (c != 'y' && c != 'Y')
    {
        printf("Account removal canceled.\n");
        sleep_sec(3);
        return;
    }

    const char *sql = "DELETE FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, usr->id);
    sqlite3_bind_int64(stmt, 2, usr->accountIds[choice]);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Failed to remove account: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);
    printf("Account number: %ld removed successfully.\n", usr->accountIds[choice]);
    get_acc_nbrs(usr, db);
}