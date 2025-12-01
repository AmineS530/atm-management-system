#include "atm_sys.h"
// account types
// place holder : https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c

//     printf("\nEnter today's date(mm/dd/yyyy):");
//     scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

//  **Transfer owner** function
void transfer_ownership(sqlite3 *db, User *usr)
{
    if (usr->accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr->name);
        return;
    }
    int choice = select_account(*usr);
    const char *sql = "UPDATE records SET userID = ? WHERE userID = ? AND accNbr = ?";
}

static int get_recepient_uid(sqlite3 *db, char *username)
{
    const char *sql = "SELECT userID FROM users WHERE username = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        return sqlite3_column_int(stmt, 0);
    
    sqlite3_finalize(stmt);
    return -1;
}