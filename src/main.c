#include "atm_sys.h"

void forexit()
{
    printOptions(3);
}

int main()
{
    User u;

    sqlite3 *db;
   // char *errMsg = 0;

    int resCode = sqlite3_open(DB_PATH, &db);
    if (resCode != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    atexit(forexit);
    initMenu(db, &u);
    mainMenu(db, u);
    sqlite3_close(db);
    return 0;
}
