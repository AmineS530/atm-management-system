#include "atm_sys.h"

void forexit();

sqlite3 *db;

void init(){
 int resCode = sqlite3_open(DB_PATH, &db);
    if (resCode != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    atexit(forexit);
}

int main()
{
    User usr;
    init();
    
    initMenu(db, &usr);
    mainMenu(db, usr);
    return 0;
}

void forexit()
{
    //  system("clear");
    sqlite3_close(db);
    printOptions(3, NULL);
}