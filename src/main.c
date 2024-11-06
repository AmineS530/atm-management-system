#include "atm_sys.h"

void forexit()
{
  //  system("clear");
    printOptions(3, NULL);
}

int main()
{
    atexit(forexit);
    User usr;
    sqlite3 *db;

    int resCode = sqlite3_open(DB_PATH, &db);
    if (resCode != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    initMenu(db, &usr);
    mainMenu(db, usr);
    sqlite3_close(db);
    return 0;
}
