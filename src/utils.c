#include "atm_sys.h"

void printOptions(int input)
{
    if (input == 1)
        printf("\n\n\t\t============== ATM ==============\n\n"
               "\n\t\t-->> Choose one of the options below <<--\n"
               "\n\t\t[1]- Create a new account\n"
               "\n\t\t[2]- Update account information\n"
               "\n\t\t[3]- Check accounts\n"
               "\n\t\t[4]- Check list of owned account\n"
               "\n\t\t[5]- Make Transaction\n"
               "\n\t\t[6]- Remove existing account\n"
               "\n\t\t[7]- Transfer ownership\n"
               "\n\t\t[8]- Exit\n");
    if (input == 2)
        printf("\n\t\t============== ATM ==============\n"
               "\n\t\t  -->>  login / register <<--\n"
               "\n\t\t[1]- login\n"
               "\n\t\t[2]- register\n"
               "\n\t\t[3]- exit\n");
    if (input == 3)
        printf("\n\n\t\t============== ATM SYS ==============\n"
               "\n\t      Thanks for using our ATM managment system\n\n");
}

// Function to check if username already exists
int username_exists(sqlite3 *db, User usr)
{
    const char *sql = "SELECT COUNT(*) FROM users WHERE username = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, usr.name, -1, SQLITE_STATIC);

    int exists = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        exists = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return exists > 0;
}
