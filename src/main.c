#include "atm_sys.h"

void forexit();

sqlite3 *db;

void init(){
 int resCode = sqlite3_open(DB_PATH, &db);
    if (resCode != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    atexit(forexit);
}

int main()
{
    User usr;
    init();
    
    init_menu(db, &usr);
    main_menu(db, usr);
    return 0;
}

void forexit()
{
    //  system("clear");
    sqlite3_close(db);
    print_options(3, NULL);
}